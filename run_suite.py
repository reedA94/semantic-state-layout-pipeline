#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import json
import logging
from collections import Counter
import os
import platform
import re
import shutil
import subprocess
import sys
from pathlib import Path
from statistics import mean
from typing import Dict, Iterable, List, Optional, Sequence, Tuple

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parent
SRC = ROOT / "src"
if str(SRC) not in sys.path:
    sys.path.insert(0, str(SRC))

from benchmarks import build_selected_benchmarks
from codegen import render_classifier_c, render_scanner_c, structural_metrics_for_layout
from core import BenchmarkSpec, DFA, granularity_implied_rollback_split
from layouts import (
    compute_class_visit_counts,
    make_arbitrary_layout,
    make_binary_threshold_layout,
    make_contiguous_interval_layout,
    make_frequency_aware_layout,
    make_hierarchical_dag_layout,
    make_hierarchical_tree_layout,
)

DEFAULT_CONFIG = ROOT / "configs" / "legacy.json"
METHOD_NAMES = {
    "B1": "lookup",
    "B2": "binary_threshold",
    "B3": "contiguous_interval",
    "B4": "frequency_interval",
    "B5_TREE": "hierarchical_tree",
    "B5_DAG": "hierarchical_dag",
    "B5": "hierarchical_tree",
}
METHOD_LABELS = {
    "B1": "arbitrary numbering + generic lookup dispatch",
    "B2": "binary final/non-final threshold encoding",
    "B3": "contiguous multi-class interval encoding",
    "B4": "frequency-aware interval encoding",
    "B5_TREE": "hierarchical comparison-tree encoding",
    "B5_DAG": "hierarchical comparison-DAG encoding",
}
LOGGER = logging.getLogger("semantic_state_layout_suite")


def run_cmd(cmd: Sequence[str], cwd: Optional[Path] = None) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, cwd=str(cwd) if cwd else None, capture_output=True, text=True, check=False)


def write_text(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")


def write_json(path: Path, payload: Dict[str, object]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, indent=2, sort_keys=True), encoding="utf-8")


def write_csv(path: Path, rows: List[Dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if not rows:
        path.write_text("", encoding="utf-8")
        return
    fieldnames: List[str] = []
    seen = set()
    for row in rows:
        for key in row.keys():
            if key not in seen:
                seen.add(key)
                fieldnames.append(key)
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        for row in rows:
            w.writerow(row)


def deep_merge(a: Dict[str, object], b: Dict[str, object]) -> Dict[str, object]:
    out = dict(a)
    for k, v in b.items():
        if isinstance(v, dict) and isinstance(out.get(k), dict):
            out[k] = deep_merge(out[k], v)
        else:
            out[k] = v
    return out


def load_config(paths: Sequence[Path]) -> Dict[str, object]:
    merged: Dict[str, object] = {}
    for path in paths:
        merged = deep_merge(merged, json.loads(path.read_text(encoding="utf-8")))
    return merged


def normalize_granularity(name: str) -> str:
    return {"default": "fine"}.get(name, name)


def benchmark_compiler_details(compiler_specs: Sequence[Dict[str, str]]) -> Dict[str, object]:
    compilers = {}
    for spec in compiler_specs:
        cc = spec["cc"]
        key = compiler_key(spec)
        path = shutil.which(cc)
        compilers[key] = {
            "cc": cc,
            "opt": spec["opt"],
            "available": bool(path),
            "path": path,
            "version": (run_cmd([cc, "--version"]).stdout.splitlines()[:1] or [""])[0] if path else "missing",
        }
    objdump = shutil.which("objdump")
    perf = shutil.which("perf")
    uname_cmd = shutil.which("uname")
    if uname_cmd:
        uname_value = run_cmd([uname_cmd, "-a"]).stdout.strip()
    else:
        u = platform.uname()
        uname_value = f"{u.system} {u.node} {u.release} {u.version} {u.machine}".strip()
    return {
        "os": platform.platform(),
        "uname": uname_value,
        "python": sys.version.replace("\n", " "),
        "compilers": compilers,
        "objdump": {"available": bool(objdump), "path": objdump, "version": (run_cmd([objdump, "--version"]).stdout.splitlines()[:1] or [""])[0] if objdump else "missing"},
        "perf": {"available": bool(perf), "path": perf},
    }


def compiler_key(spec: Dict[str, str]) -> str:
    return f"{spec['cc']}_{spec['opt'].replace('-', '')}"


def compiler_flags(spec: Dict[str, str]) -> List[str]:
    return [spec["opt"], "-std=c11", "-Wall", "-Wextra", "-pedantic"]


def scan_python(dfa: DFA, text: str) -> Dict[str, object]:
    buf = text.encode("utf-8")
    p = 0
    visited_states: List[int] = []
    tokens = 0
    errors = 0
    while p < len(buf):
        q = dfa.start_state
        i = p
        last_accept = -1
        while i < len(buf):
            q = dfa.transitions[q][buf[i]]
            visited_states.append(q)
            if dfa.states[q].accepting:
                last_accept = i
            if dfa.states[q].dead:
                break
            i += 1
        if last_accept >= p:
            tokens += 1
            p = last_accept + 1
        else:
            errors += 1
            p += 1
    return {"visited_states": visited_states, "tokens": tokens, "errors": errors, "input_bytes": len(buf)}


def benchmark_rollback_metadata(benchmark: BenchmarkSpec) -> Dict[str, object]:
    explicit = benchmark.metadata.get("rollback_metadata_available")
    if explicit is None:
        explicit = any(getattr(st, "accepting", False) for st in benchmark.dfa.states)
    families = {st.family for st in benchmark.dfa.states if st.accepting and st.family}
    return {
        "rollback_metadata_available": bool(explicit),
        "num_accepting_states_total": sum(1 for st in benchmark.dfa.states if st.accepting),
        "num_accepting_states_rb": sum(1 for st in benchmark.dfa.states if st.accepting and st.rollback),
        "num_accepting_states_nonrb": sum(1 for st in benchmark.dfa.states if st.accepting and not st.rollback),
        "num_token_families_with_rb_split": len({
            fam for fam in families
            if any(st.accepting and st.family == fam and st.rollback for st in benchmark.dfa.states)
            and any(st.accepting and st.family == fam and not st.rollback for st in benchmark.dfa.states)
        }),
    }


def semantic_mode_name(granularity_name: str, granularity: str, rollback_split: str) -> str:
    if granularity_name in {"operational_rb_off", "operational_rb_on"}:
        return granularity_name
    if granularity == "operational":
        return f"operational_rb_{rollback_split}"
    return granularity_name


def effective_rollback_conditions(config: Dict[str, object], granularity_name: str, granularity: str) -> List[str]:
    implied = granularity_implied_rollback_split(granularity_name)
    if implied is not None:
        return [implied]
    declared = list(config.get("rollback_split_conditions", []))
    if not declared:
        return ["auto"]
    if granularity not in {"coarse", "operational", "fine"}:
        return ["off"]
    return declared


def build_layouts(benchmark: BenchmarkSpec, granularity: str, profile_source: str, *, rollback_split: str, rollback_metadata_available: bool) -> Dict[str, object]:
    class_of_old = benchmark.dfa.semantic_classes(
        granularity=granularity,
        rollback_split=rollback_split,
        rollback_metadata_available=rollback_metadata_available,
    )
    profile_text = benchmark.profile_inputs[profile_source]
    profile_trace = scan_python(benchmark.dfa, profile_text)
    profile_counts = compute_class_visit_counts(class_of_old, profile_trace["visited_states"])
    state_visit_counts = Counter(profile_trace["visited_states"])
    layouts = {
        "B1": make_arbitrary_layout(benchmark.dfa, class_of_old, granularity=granularity, seed=0),
        "B2": make_binary_threshold_layout(benchmark.dfa, class_of_old, granularity=granularity),
        "B3": make_contiguous_interval_layout(benchmark.dfa, class_of_old, granularity=granularity),
        "B4": make_frequency_aware_layout(benchmark.dfa, class_of_old, granularity=granularity, class_visit_counts=profile_counts, profile_source=profile_source),
        "B5_TREE": make_hierarchical_tree_layout(benchmark.dfa, class_of_old, granularity=granularity),
        "B5_DAG": make_hierarchical_dag_layout(benchmark.dfa, class_of_old, granularity=granularity),
    }
    layouts["B5"] = layouts["B5_TREE"]
    return {
        "class_of_old": class_of_old,
        "profile_trace": profile_trace,
        "profile_counts": profile_counts,
        "state_visit_counts": state_visit_counts,
        "layouts": layouts,
    }


def compile_binary(scanner_c: Path, binary_path: Path, compiler_spec: Dict[str, str]) -> Tuple[bool, str]:
    cmd = [compiler_spec["cc"], *compiler_flags(compiler_spec), "-o", str(binary_path), str(scanner_c)]
    proc = run_cmd(cmd)
    log = "$ " + " ".join(cmd) + "\n\nSTDOUT:\n" + proc.stdout + "\nSTDERR:\n" + proc.stderr
    return proc.returncode == 0, log


def parse_instruction_count(disassembly: str, function_name: str) -> int:
    m = re.search(rf"^[0-9a-f]+ <{re.escape(function_name)}>:\n(?P<body>(?:\s+[0-9a-f]+:.*\n)+)", disassembly, re.MULTILINE)
    if not m:
        return -1
    body = m.group("body")
    count = 0
    for line in body.splitlines():
        if re.match(r"\s+[0-9a-f]+:\s+[0-9a-f ]+\s+\S+", line):
            count += 1
    return count


def aggregate_metrics(rows: List[Dict[str, object]]) -> Dict[str, object]:
    total_bytes = sum(int(r["input_bytes"]) for r in rows)
    total_tokens = sum(int(r["emitted_tokens"]) for r in rows)
    total_errors = sum(int(r["lexical_errors"]) for r in rows)
    total_full_ns = sum(int(r["full_scan_ns"]) for r in rows)
    total_cls_calls = sum(int(r["classify_calls"]) for r in rows)
    total_trans_calls = sum(int(r["transition_calls"]) for r in rows)
    total_sem_cmp = sum(int(r["semantic_comparisons"]) for r in rows)
    total_cls_trace_ops = sum(int(r["trace_len"]) * int(r["classifier_trace_loops"]) for r in rows)
    total_trans_trace_ops = sum(int(r["trace_len"]) * int(r["transition_trace_loops"]) for r in rows)
    total_cls_trace_ns = sum(int(r["classifier_trace_ns"]) for r in rows)
    total_trans_trace_ns = sum(int(r["transition_trace_ns"]) for r in rows)
    classifier_ns_per_call = (total_cls_trace_ns / total_cls_trace_ops) if total_cls_trace_ops else 0.0
    transition_ns_per_call = (total_trans_trace_ns / total_trans_trace_ops) if total_trans_trace_ops else 0.0
    est_cls_share = ((classifier_ns_per_call * total_cls_calls) / total_full_ns) if total_full_ns else 0.0
    est_trans_share = ((transition_ns_per_call * total_trans_calls) / total_full_ns) if total_full_ns else 0.0
    return {
        "num_inputs": len(rows),
        "input_bytes_total": total_bytes,
        "emitted_tokens_total": total_tokens,
        "lexical_errors_total": total_errors,
        "full_scan_ns_total": total_full_ns,
        "classifier_trace_ns_total": total_cls_trace_ns,
        "transition_trace_ns_total": total_trans_trace_ns,
        "classify_calls_total": total_cls_calls,
        "transition_calls_total": total_trans_calls,
        "semantic_comparisons_total": total_sem_cmp,
        "bytes_per_second_aggregate": (total_bytes / (total_full_ns / 1e9)) if total_full_ns else 0.0,
        "tokens_per_second_aggregate": (total_tokens / (total_full_ns / 1e9)) if total_full_ns else 0.0,
        "avg_semantic_comparisons_per_visited_state": (total_sem_cmp / total_cls_calls) if total_cls_calls else 0.0,
        "avg_semantic_comparisons_per_emitted_token": (total_sem_cmp / total_tokens) if total_tokens else 0.0,
        "classifier_ns_per_call": classifier_ns_per_call,
        "transition_ns_per_call": transition_ns_per_call,
        "estimated_classification_share_of_full_scan": est_cls_share,
        "estimated_transition_share_of_full_scan": est_trans_share,
        "mean_input_bytes": mean(int(r["input_bytes"]) for r in rows),
    }


def should_skip(result_dir: Path, run_config: Dict[str, object], force: bool, resume: bool) -> bool:
    metrics_path = result_dir / "metrics.json"
    config_path = result_dir / "run_config.json"
    if force or not resume:
        return False
    if not metrics_path.exists() or not config_path.exists():
        return False
    try:
        old = json.loads(config_path.read_text(encoding="utf-8"))
    except Exception:
        return False
    if old != run_config:
        return False
    try:
        metrics = json.loads(metrics_path.read_text(encoding="utf-8"))
    except Exception:
        return False
    return bool(metrics.get("completed", False) or metrics.get("compile_failed", False))


def discover_eval_inputs(benchmark: BenchmarkSpec, group: str) -> List[Tuple[str, str]]:
    if group == "*":
        return sorted(benchmark.eval_inputs.items())
    names = benchmark.eval_groups.get(group, [])
    return [(name, benchmark.eval_inputs[name]) for name in names]




def ingest_existing_result(result_dir: Path) -> Dict[str, object]:
    metrics = json.loads((result_dir / "metrics.json").read_text(encoding="utf-8"))
    perf_payload = {"perf_available": False, "perf_status": "missing"}
    perf_path = result_dir / "perf_metrics.json"
    if perf_path.exists():
        perf_payload = json.loads(perf_path.read_text(encoding="utf-8"))
    return {"metrics": metrics, "perf": perf_payload}


def perf_available() -> Tuple[bool, str]:
    perf = shutil.which("perf")
    if not perf:
        return False, "perf_not_found"
    proc = run_cmd([perf, "stat", "true"])
    if proc.returncode == 0:
        return True, "ok"
    return False, (proc.stderr.strip() or proc.stdout.strip() or "perf_unavailable")


def run_perf(binary_path: Path, input_path: Path) -> Dict[str, object]:
    perf = shutil.which("perf")
    if not perf:
        return {"perf_available": False, "perf_status": "perf_not_found"}
    cmd = [perf, "stat", "-x,", "-e", "cycles,instructions,branches,branch-misses", str(binary_path), "--mode", "perf_fullscan", str(input_path)]
    proc = run_cmd(cmd)
    if proc.returncode != 0:
        return {"perf_available": False, "perf_status": (proc.stderr.strip() or proc.stdout.strip() or "perf_failed")}
    counters: Dict[str, object] = {"perf_available": True, "perf_status": "ok"}
    metric_key = {"cycles": "cycles", "instructions": "instructions", "branches": "branches", "branch-misses": "branch_misses"}
    for line in proc.stderr.splitlines():
        parts = [x.strip() for x in line.split(",")]
        if len(parts) >= 3 and parts[2] in metric_key:
            raw = parts[0]
            key = metric_key[parts[2]]
            if raw in {"<not supported>", "<not counted>", ""}:
                counters[key] = None
            else:
                try:
                    counters[key] = int(raw)
                except ValueError:
                    try:
                        counters[key] = float(raw)
                    except ValueError:
                        counters[key] = raw
    counters["perf_stdout"] = proc.stdout.strip()
    return counters


def safe_binary_size(path: Path) -> int:
    try:
        return path.stat().st_size
    except OSError:
        return -1


def legacy_result_dir(raw_dir: Path, benchmark_id: str, granularity: str, method_id: str) -> Path:
    return raw_dir / benchmark_id / granularity / METHOD_NAMES[method_id]


def rich_result_dir(raw_dir: Path, benchmark_id: str, granularity: str, method_id: str, compiler_id: str, scenario_name: str) -> Path:
    return raw_dir / benchmark_id / granularity / METHOD_NAMES[method_id] / compiler_id / scenario_name


def create_plots(structural_rows: List[Dict[str, object]], runtime_rows: List[Dict[str, object]], profile_rows: List[Dict[str, object]], plots_dir: Path) -> None:
    plots_dir.mkdir(parents=True, exist_ok=True)
    structural_rows = [r for r in structural_rows if r.get("status") == "completed"]
    runtime_rows = [r for r in runtime_rows if r.get("status") == "completed"]
    profile_rows = [r for r in profile_rows if r.get("status") == "completed"]
    if not structural_rows or not runtime_rows:
        return

    def _pick_default_runtime_rows() -> List[Dict[str, object]]:
        rows = [
            r for r in runtime_rows
            if r.get("granularity") in {"fine", "default", "operational"}
            and r.get("compiler_id") == "gcc_O3"
            and r.get("profile_condition") == "same_A"
            and r.get("rollback_split_condition") in {"auto", "off"}
        ]
        return rows

    def _pick_default_struct_rows() -> List[Dict[str, object]]:
        rows = [
            r for r in structural_rows
            if r.get("granularity") in {"fine", "default", "operational"}
            and r.get("compiler_id") == "gcc_O3"
            and r.get("profile_condition") == "same_A"
            and r.get("rollback_split_condition") in {"auto", "off"}
        ]
        return rows

    def _group(metric: str, rows: List[Dict[str, object]], ylabel: str, filename: str, methods: Optional[List[str]] = None) -> None:
        if not rows:
            return
        benchmarks = sorted({r["benchmark_id"] for r in rows})
        methods = methods or sorted({r["method_id"] for r in rows})
        x = list(range(len(benchmarks)))
        width = 0.8 / max(1, len(methods))
        plt.figure(figsize=(12, 5))
        for idx, method in enumerate(methods):
            vals = []
            for bench in benchmarks:
                hit = next((r for r in rows if r["benchmark_id"] == bench and r["method_id"] == method), None)
                vals.append(float(hit.get(metric, 0.0) or 0.0) if hit else 0.0)
            xs = [v - 0.4 + width / 2 + idx * width for v in x]
            plt.bar(xs, vals, width=width, label=method)
        plt.xticks(x, benchmarks, rotation=45)
        plt.ylabel(ylabel)
        plt.xlabel("Benchmark")
        plt.legend(ncols=min(4, len(methods)), fontsize=8)
        plt.tight_layout()
        plt.savefig(plots_dir / f"{filename}.png", dpi=180)
        plt.savefig(plots_dir / f"{filename}.svg")
        plt.close()

    _group("avg_semantic_comparisons_per_visited_state", _pick_default_runtime_rows(), "Avg semantic comparisons / visited state", "dispatch_depth_by_method")
    _group("throughput_bytes_per_sec", _pick_default_runtime_rows(), "Bytes per second", "runtime_by_method")
    _group("binary_size_bytes", _pick_default_struct_rows(), "Binary size (bytes)", "code_size_by_method")

    rows = [r for r in profile_rows if r.get("compiler_id") == "gcc_O3" and r.get("granularity") == "fine" and r.get("method_id") in {"B3", "B4"} and r.get("rollback_split_condition") in {"auto", "off"}]
    if rows:
        benchmarks = sorted({r["benchmark_id"] for r in rows})
        scenarios = sorted({r["profile_condition"] for r in rows})
        plt.figure(figsize=(12, 5))
        width = 0.8 / max(1, len(scenarios))
        x = list(range(len(benchmarks)))
        for idx, scenario in enumerate(scenarios):
            vals = []
            for bench in benchmarks:
                hit = next((r for r in rows if r["benchmark_id"] == bench and r["profile_condition"] == scenario and r["method_id"] == "B4"), None)
                vals.append(float(hit.get("throughput_bytes_per_sec", 0.0) or 0.0) if hit else 0.0)
            xs = [v - 0.4 + width / 2 + idx * width for v in x]
            plt.bar(xs, vals, width=width, label=scenario)
        plt.xticks(x, benchmarks, rotation=45)
        plt.ylabel("B4 bytes per second")
        plt.xlabel("Benchmark")
        plt.legend(fontsize=8)
        plt.tight_layout()
        plt.savefig(plots_dir / "profile_shift_b4.png", dpi=180)
        plt.savefig(plots_dir / "profile_shift_b4.svg")
        plt.close()

    compiler_rows = [r for r in runtime_rows if r.get("granularity") == "fine" and r.get("method_id") == "B4" and r.get("profile_condition") == "same_A" and r.get("rollback_split_condition") in {"auto", "off"}]
    if compiler_rows:
        benchmarks = sorted({r["benchmark_id"] for r in compiler_rows})
        compilers = sorted({r["compiler_id"] for r in compiler_rows})
        plt.figure(figsize=(12, 5))
        width = 0.8 / max(1, len(compilers))
        x = list(range(len(benchmarks)))
        for idx, compiler_id in enumerate(compilers):
            vals = []
            for bench in benchmarks:
                hit = next((r for r in compiler_rows if r["benchmark_id"] == bench and r["compiler_id"] == compiler_id), None)
                vals.append(float(hit.get("throughput_bytes_per_sec", 0.0) or 0.0) if hit else 0.0)
            xs = [v - 0.4 + width / 2 + idx * width for v in x]
            plt.bar(xs, vals, width=width, label=compiler_id)
        plt.xticks(x, benchmarks, rotation=45)
        plt.ylabel("B4 bytes per second")
        plt.xlabel("Benchmark")
        plt.legend(fontsize=8)
        plt.tight_layout()
        plt.savefig(plots_dir / "compiler_sensitivity.png", dpi=180)
        plt.savefig(plots_dir / "compiler_sensitivity.svg")
        plt.close()

    rollback_rows = [r for r in structural_rows if r.get("rollback_split_condition") in {"off", "on"}]
    keyed: Dict[tuple, Dict[str, Dict[str, object]]] = {}
    for row in rollback_rows:
        key = (row.get("benchmark_id"), row.get("method_id"), row.get("compiler_id"), row.get("profile_condition"), row.get("granularity"))
        keyed.setdefault(key, {})[str(row.get("rollback_split_condition"))] = row
    delta_struct = []
    delta_accept = []
    delta_rb = []
    delta_thr = []
    for key, pair in keyed.items():
        off = pair.get("off")
        on = pair.get("on")
        if not off or not on:
            continue
        label = f"{key[0]}::{key[1]}"
        delta_struct.append((label, float(on.get("comparison_sites", 0) or 0) - float(off.get("comparison_sites", 0) or 0)))
        delta_accept.append((label, float(on.get("accept_predicate_expected_depth", 0.0) or 0.0)))
        delta_rb.append((label, float(on.get("rollbackacc_predicate_expected_depth", 0.0) or 0.0)))
        on_runtime = next((r for r in runtime_rows if r.get("benchmark_id") == key[0] and r.get("method_id") == key[1] and r.get("compiler_id") == key[2] and r.get("profile_condition") == key[3] and r.get("granularity") == key[4] and r.get("rollback_split_condition") == "on"), None)
        off_runtime = next((r for r in runtime_rows if r.get("benchmark_id") == key[0] and r.get("method_id") == key[1] and r.get("compiler_id") == key[2] and r.get("profile_condition") == key[3] and r.get("granularity") == key[4] and r.get("rollback_split_condition") == "off"), None)
        if on_runtime and off_runtime:
            delta_thr.append((label, float(on_runtime.get("throughput_bytes_per_sec", 0.0) or 0.0) - float(off_runtime.get("throughput_bytes_per_sec", 0.0) or 0.0)))

    def _simple_delta_plot(points: List[tuple], ylabel: str, filename: str) -> None:
        if not points:
            return
        labels = [p[0] for p in points]
        vals = [p[1] for p in points]
        plt.figure(figsize=(max(10, len(labels) * 0.35), 5))
        x = list(range(len(labels)))
        plt.bar(x, vals)
        plt.xticks(x, labels, rotation=70, ha="right")
        plt.ylabel(ylabel)
        plt.xlabel("Benchmark::Method")
        plt.tight_layout()
        plt.savefig(plots_dir / f"{filename}.png", dpi=180)
        plt.savefig(plots_dir / f"{filename}.svg")
        plt.close()

    _simple_delta_plot(delta_struct, "Rollback on - off comparison sites", "rollback_ablation_structural_delta")
    _simple_delta_plot(delta_accept, "Accept(s) expected depth", "accept_predicate_cost")
    _simple_delta_plot(delta_rb, "RollbackAcc(s) expected depth", "rollbackacc_predicate_cost")
    _simple_delta_plot(delta_thr, "Rollback on - off bytes/s", "rollback_ablation_throughput_delta")


def build_readme(env: Dict[str, object], config: Dict[str, object]) -> str:
    return f"""# Semantic state-layout synthesis benchmark suite

This artifact implements a reproducible benchmark pipeline for the manuscript on semantic state-layout synthesis for generated lexers / DFAs.

## Methods

- B1: arbitrary numbering + generic lookup dispatch
- B2: binary final/non-final threshold encoding
- B3: contiguous multi-class interval encoding
- B4: frequency-aware interval encoding
- B5_TREE: hierarchical comparison-tree encoding
- B5_DAG: hierarchical comparison-DAG encoding

## Benchmarks

The registry now includes BM1-BM7 plus synthetic families BM_SYN_S / BM_SYN_M / BM_SYN_L.

## Configuration entry point

```bash
python run_suite.py --config configs/full.json
```

## Current default config

`{config.get('name', 'unnamed')}`

## Output structure

- `corpora/<benchmark>/profiles/` and `corpora/<benchmark>/eval/`
- `raw_results/<benchmark>/<granularity>/<method>/<compiler>/<profile_scenario>/`
- `summaries/`
- `plots/`
- `environment.json`

## Environment snapshot

- OS: {env['os']}
- Python: {env['python']}
- Compilers: {", ".join(sorted(env['compilers'].keys()))}
- perf available: {env['perf']['available']}
"""


def make_logger(log_path: Path) -> None:
    LOGGER.setLevel(logging.INFO)
    LOGGER.handlers.clear()
    fmt = logging.Formatter("%(asctime)s | %(levelname)s | %(message)s")
    sh = logging.StreamHandler(sys.stdout)
    sh.setFormatter(fmt)
    fh = logging.FileHandler(log_path, encoding="utf-8")
    fh.setFormatter(fmt)
    LOGGER.addHandler(sh)
    LOGGER.addHandler(fh)


def main() -> int:
    parser = argparse.ArgumentParser(description="Semantic state-layout synthesis benchmark suite")
    parser.add_argument("--config", action="append", help="JSON config file(s)")
    parser.add_argument("--force", action="store_true", help="Ignore resume logic and rerun")
    args = parser.parse_args()

    config_paths = [Path(p) for p in (args.config or [str(DEFAULT_CONFIG)])]
    config = load_config(config_paths)
    raw_dir = ROOT / "raw_results"
    corpora_dir = ROOT / "corpora"
    summaries_dir = ROOT / "summaries"
    plots_dir = ROOT / "plots"
    logs_dir = ROOT / "logs"
    logs_dir.mkdir(parents=True, exist_ok=True)
    make_logger(logs_dir / "run.log")

    benchmark_ids = config["benchmarks"]
    benchmarks = build_selected_benchmarks(benchmark_ids)
    compiler_specs = config["compilers"]
    env = benchmark_compiler_details(compiler_specs)
    write_json(ROOT / "environment.json", env)
    write_text(ROOT / "README.md", build_readme(env, config))

    if config.get("clean", False):
        for path in [raw_dir, corpora_dir, summaries_dir, plots_dir]:
            if path.exists():
                shutil.rmtree(path)
    for path in [raw_dir, corpora_dir, summaries_dir, plots_dir]:
        path.mkdir(parents=True, exist_ok=True)

    perf_ok, perf_status = perf_available()
    LOGGER.info("perf availability: %s (%s)", perf_ok, perf_status)

    benchmark_rows: List[Dict[str, object]] = []
    structural_rows: List[Dict[str, object]] = []
    runtime_rows: List[Dict[str, object]] = []
    code_size_rows: List[Dict[str, object]] = []
    perf_rows: List[Dict[str, object]] = []
    profile_rows: List[Dict[str, object]] = []
    ablation_rows: List[Dict[str, object]] = []

    def append_condition_rows(metrics_json: Dict[str, object], perf_payload: Dict[str, object]) -> None:
        flat = {k: v for k, v in metrics_json.items() if not isinstance(v, (dict, list))}
        structural_rows.append(flat)
        runtime_rows.append({
            "benchmark_id": flat.get("benchmark_id"),
            "benchmark_family": flat.get("benchmark_family"),
            "granularity": flat.get("granularity"),
            "granularity_normalized": flat.get("granularity_normalized"),
            "semantic_label_mode": flat.get("semantic_label_mode"),
            "rollback_split_condition": flat.get("rollback_split_condition"),
            "method_id": flat.get("method_id"),
            "method_label": flat.get("method_label"),
            "compiler": flat.get("compiler"),
            "compiler_id": flat.get("compiler_id"),
            "opt_level": flat.get("opt_level"),
            "profile_source": flat.get("profile_source"),
            "profile_condition": flat.get("profile_condition"),
            "profile_scenario": flat.get("profile_scenario"),
            "eval_source": flat.get("eval_source"),
            "eval_group": flat.get("eval_group"),
            "status": flat.get("status"),
            "error_message": flat.get("error_message"),
            "avg_semantic_comparisons_per_visited_state": flat.get("avg_semantic_comparisons_per_visited_state"),
            "avg_semantic_comparisons_per_emitted_token": flat.get("avg_semantic_comparisons_per_emitted_token"),
            "throughput_bytes_per_sec": flat.get("throughput_bytes_per_sec"),
            "bytes_per_second_aggregate": flat.get("bytes_per_second_aggregate"),
            "tokens_per_second_aggregate": flat.get("tokens_per_second_aggregate"),
            "classifier_ns_per_call": flat.get("classifier_ns_per_call"),
            "transition_ns_per_call": flat.get("transition_ns_per_call"),
            "estimated_classification_share_of_full_scan": flat.get("estimated_classification_share_of_full_scan"),
            "estimated_transition_share_of_full_scan": flat.get("estimated_transition_share_of_full_scan"),
            "full_scan_ns_total": flat.get("full_scan_ns_total"),
            "emitted_tokens_total": flat.get("emitted_tokens_total"),
            "input_bytes_total": flat.get("input_bytes_total"),
        })
        code_size_rows.append({
            "benchmark_id": flat.get("benchmark_id"),
            "granularity": flat.get("granularity"),
            "semantic_label_mode": flat.get("semantic_label_mode"),
            "rollback_split_condition": flat.get("rollback_split_condition"),
            "method_id": flat.get("method_id"),
            "compiler": flat.get("compiler"),
            "compiler_id": flat.get("compiler_id"),
            "opt_level": flat.get("opt_level"),
            "profile_condition": flat.get("profile_condition"),
            "status": flat.get("status"),
            "error_message": flat.get("error_message"),
            "classifier_source_bytes": flat.get("classifier_source_bytes"),
            "generated_classifier_source_size_bytes": flat.get("generated_classifier_source_size_bytes"),
            "generated_scanner_source_size_bytes": flat.get("generated_scanner_source_size_bytes"),
            "compiled_instruction_count": flat.get("compiled_instruction_count"),
            "classifier_instruction_count": flat.get("classifier_instruction_count"),
            "transition_instruction_count": flat.get("transition_instruction_count"),
            "binary_size_bytes": flat.get("binary_size_bytes"),
        })
        perf_rows.append({
            "benchmark_id": flat.get("benchmark_id"),
            "granularity": flat.get("granularity"),
            "semantic_label_mode": flat.get("semantic_label_mode"),
            "rollback_split_condition": flat.get("rollback_split_condition"),
            "method_id": flat.get("method_id"),
            "compiler": flat.get("compiler"),
            "compiler_id": flat.get("compiler_id"),
            "opt_level": flat.get("opt_level"),
            "profile_condition": flat.get("profile_condition"),
            "status": flat.get("status"),
            "error_message": flat.get("error_message"),
            **perf_payload,
        })
        if flat.get("method_id") in {"B3", "B4"}:
            profile_rows.append({
                "benchmark_id": flat.get("benchmark_id"),
                "granularity": flat.get("granularity"),
                "semantic_label_mode": flat.get("semantic_label_mode"),
                "rollback_split_condition": flat.get("rollback_split_condition"),
                "method_id": flat.get("method_id"),
                "compiler": flat.get("compiler"),
                "compiler_id": flat.get("compiler_id"),
                "opt_level": flat.get("opt_level"),
                "profile_source": flat.get("profile_source"),
                "profile_condition": flat.get("profile_condition"),
                "profile_scenario": flat.get("profile_scenario"),
                "eval_source": flat.get("eval_source"),
                "status": flat.get("status"),
                "error_message": flat.get("error_message"),
                "throughput_bytes_per_sec": flat.get("throughput_bytes_per_sec"),
                "avg_semantic_comparisons_per_visited_state": flat.get("avg_semantic_comparisons_per_visited_state"),
            })
        ablation_rows.append({
            "benchmark_id": flat.get("benchmark_id"),
            "benchmark_family": flat.get("benchmark_family"),
            "granularity": flat.get("granularity"),
            "semantic_label_mode": flat.get("semantic_label_mode"),
            "rollback_split_condition": flat.get("rollback_split_condition"),
            "method_id": flat.get("method_id"),
            "compiler": flat.get("compiler"),
            "compiler_id": flat.get("compiler_id"),
            "opt_level": flat.get("opt_level"),
            "profile_condition": flat.get("profile_condition"),
            "status": flat.get("status"),
            "error_message": flat.get("error_message"),
            "comparison_sites": flat.get("comparison_sites"),
            "worst_case_depth": flat.get("worst_case_depth"),
            "expected_depth": flat.get("expected_depth"),
            "accept_predicate_num_intervals": flat.get("accept_predicate_num_intervals"),
            "accept_predicate_comparison_sites": flat.get("accept_predicate_comparison_sites"),
            "accept_predicate_worst_case_depth": flat.get("accept_predicate_worst_case_depth"),
            "accept_predicate_expected_depth": flat.get("accept_predicate_expected_depth"),
            "rollbackacc_predicate_num_intervals": flat.get("rollbackacc_predicate_num_intervals"),
            "rollbackacc_predicate_comparison_sites": flat.get("rollbackacc_predicate_comparison_sites"),
            "rollbackacc_predicate_worst_case_depth": flat.get("rollbackacc_predicate_worst_case_depth"),
            "rollbackacc_predicate_expected_depth": flat.get("rollbackacc_predicate_expected_depth"),
            "throughput_bytes_per_sec": flat.get("throughput_bytes_per_sec"),
        })

    for benchmark in benchmarks:
        LOGGER.info("Preparing benchmark %s", benchmark.benchmark_id)
        rollback_meta = benchmark_rollback_metadata(benchmark)
        bench_corpus_dir = corpora_dir / benchmark.benchmark_id
        for pname, ptext in benchmark.profile_inputs.items():
            write_text(bench_corpus_dir / "profiles" / f"{pname}.txt", ptext)
        for ename, etext in benchmark.eval_inputs.items():
            write_text(bench_corpus_dir / "eval" / f"{ename}.txt", etext)
        benchmark_rows.append({
            "benchmark_id": benchmark.benchmark_id,
            "benchmark_family": benchmark.family,
            "num_states": benchmark.dfa.num_states,
            "notes": benchmark.notes,
            "profile_sources": ";".join(sorted(benchmark.profile_inputs)),
            "eval_groups": ";".join(sorted(benchmark.eval_groups)),
            **rollback_meta,
            **{f"meta_{k}": v for k, v in benchmark.metadata.items() if not isinstance(v, (dict, list))},
        })

        for granularity_name in config["granularities"]:
            granularity = normalize_granularity(granularity_name)
            rollback_conditions = effective_rollback_conditions(config, granularity_name, granularity)
            for rollback_split in rollback_conditions:
                semantic_mode = semantic_mode_name(granularity_name, granularity, rollback_split)
                for scenario in config["profile_scenarios"]:
                    profile_source = scenario["profile_source"]
                    eval_group = scenario["eval_group"]
                    scenario_name = scenario["name"]
                    scenario_dir_name = scenario_name if rollback_split == "auto" and not config.get("rollback_split_conditions") else f"{scenario_name}__rb_{rollback_split}"
                    if profile_source not in benchmark.profile_inputs or (eval_group != "*" and eval_group not in benchmark.eval_groups):
                        LOGGER.info("Skipping scenario %s for %s: unavailable profile/group", scenario_name, benchmark.benchmark_id)
                        continue
                    eval_inputs = discover_eval_inputs(benchmark, eval_group)
                    base_condition = {
                        "benchmark_id": benchmark.benchmark_id,
                        "benchmark_family": benchmark.family,
                        "granularity": granularity_name,
                        "granularity_normalized": granularity,
                        "semantic_label_mode": semantic_mode,
                        "rollback_split_condition": rollback_split,
                        "profile_source": profile_source,
                        "profile_condition": scenario_name,
                        "profile_scenario": scenario_name,
                        "eval_source": eval_group,
                        "eval_group": eval_group,
                        "seed": benchmark.metadata.get("seed", 0),
                        **rollback_meta,
                    }

                    missing_rb_metadata = rollback_split == "on" and granularity in {"coarse", "operational", "fine"} and not rollback_meta["rollback_metadata_available"]
                    if missing_rb_metadata:
                        for method_raw in config["methods"]:
                            method_id = "B5_TREE" if method_raw == "B5" else method_raw
                            method_label = METHOD_LABELS.get(method_id, method_id)
                            for compiler_spec in compiler_specs:
                                compiler_id = compiler_key(compiler_spec)
                                result_dir = rich_result_dir(raw_dir, benchmark.benchmark_id, granularity_name, method_id, compiler_id, scenario_dir_name)
                                result_dir.mkdir(parents=True, exist_ok=True)
                                run_config = {**base_condition, "method_id": method_id, "method_label": method_label, "compiler": compiler_spec, "compiler_id": compiler_id, "opt_level": compiler_spec["opt"], "eval_inputs": [name for name, _ in eval_inputs]}
                                write_json(result_dir / "run_config.json", run_config)
                                metrics_json = {**base_condition, "method_id": method_id, "method_label": method_label, "compiler": compiler_spec["cc"], "compiler_id": compiler_id, "opt_level": compiler_spec["opt"], "status": "skipped_missing_rollback_metadata", "error_message": "rollback metadata unavailable for benchmark", "compile_failed": False, "completed": False}
                                perf_payload = {"perf_available": False, "perf_status": "rollback_metadata_unavailable"}
                                write_json(result_dir / "metrics.json", metrics_json)
                                write_json(result_dir / "perf_metrics.json", perf_payload)
                                append_condition_rows(metrics_json, perf_payload)
                        continue

                    ctx = build_layouts(benchmark, granularity=granularity, profile_source=profile_source, rollback_split=rollback_split, rollback_metadata_available=bool(rollback_meta["rollback_metadata_available"]))
                    state_visit_counts_old = [int(ctx["state_visit_counts"].get(i, 0)) for i in range(benchmark.dfa.num_states)]
                    for method_raw in config["methods"]:
                        method_id = "B5_TREE" if method_raw == "B5" else method_raw
                        layout = ctx["layouts"][method_id]
                        for compiler_spec in compiler_specs:
                            compiler_id = compiler_key(compiler_spec)
                            if config.get("legacy_output_compat", False) and compiler_id == "gcc_O3" and scenario_name == "same_A" and rollback_split == "auto":
                                result_dir = legacy_result_dir(raw_dir, benchmark.benchmark_id, granularity_name, method_id)
                            else:
                                result_dir = rich_result_dir(raw_dir, benchmark.benchmark_id, granularity_name, method_id, compiler_id, scenario_dir_name)
                            result_dir.mkdir(parents=True, exist_ok=True)

                            classifier_c = render_classifier_c(layout, benchmark.dfa)
                            scanner_c = render_scanner_c(layout, benchmark.dfa)
                            run_config = {
                                **base_condition,
                                "method_id": method_id,
                                "method_label": layout.method_label,
                                "compiler": compiler_spec,
                                "compiler_id": compiler_id,
                                "opt_level": compiler_spec["opt"],
                                "eval_inputs": [name for name, _ in eval_inputs],
                                "layout_metadata": layout.metadata,
                                "profile_counts": ctx["profile_counts"],
                                "profile_trace_len": len(ctx["profile_trace"]["visited_states"]),
                            }
                            write_json(result_dir / "run_config.json", run_config)
                            if should_skip(result_dir, run_config, args.force, bool(config.get("resume", True))):
                                LOGGER.info("Skipping completed run %s | %s | %s | %s | %s | rb=%s", benchmark.benchmark_id, granularity_name, method_id, compiler_id, scenario_name, rollback_split)
                                existing = ingest_existing_result(result_dir)
                                append_condition_rows(existing["metrics"], existing["perf"])
                                continue

                            compiler_info = env["compilers"][compiler_id]
                            if not compiler_info["available"]:
                                LOGGER.info("Compiler %s unavailable; recording skip", compiler_id)
                                metrics_json = {
                                    **base_condition,
                                    "method_id": method_id,
                                    "method_label": layout.method_label,
                                    "compiler": compiler_spec["cc"],
                                    "compiler_id": compiler_id,
                                    "opt_level": compiler_spec["opt"],
                                    "status": "skipped_unavailable_compiler",
                                    "error_message": f"compiler unavailable: {compiler_id}",
                                    "compile_failed": False,
                                    "completed": False,
                                }
                                perf_payload = {"perf_available": False, "perf_status": "compiler_unavailable"}
                                write_json(result_dir / "metrics.json", metrics_json)
                                write_json(result_dir / "perf_metrics.json", perf_payload)
                                append_condition_rows(metrics_json, perf_payload)
                                continue

                            write_text(result_dir / "generated_classifier.c", classifier_c)
                            write_text(result_dir / "generated_scanner.c", scanner_c)
                            struct = structural_metrics_for_layout(
                                benchmark.dfa,
                                layout,
                                state_visit_counts_old=state_visit_counts_old,
                                rollback_metadata_available=bool(rollback_meta["rollback_metadata_available"]),
                            )
                            struct.update({
                                **base_condition,
                                "method_id": method_id,
                                "method_label": layout.method_label,
                                "compiler": compiler_spec["cc"],
                                "compiler_id": compiler_id,
                                "opt_level": compiler_spec["opt"],
                                "profile_input_bytes": ctx["profile_trace"]["input_bytes"],
                                "profile_trace_len": len(ctx["profile_trace"]["visited_states"]),
                                "classifier_source_bytes": len(classifier_c.encode("utf-8")),
                                "generated_classifier_source_size_bytes": len(classifier_c.encode("utf-8")),
                                "generated_scanner_source_size_bytes": len(scanner_c.encode("utf-8")),
                                "frequency_source": f"profile {profile_source}",
                            })
                            LOGGER.info("Running %s | %s | %s | %s | %s | rb=%s", benchmark.benchmark_id, granularity_name, method_id, compiler_id, scenario_name, rollback_split)
                            binary_path = result_dir / "scanner_bench"
                            ok, compile_log = compile_binary(result_dir / "generated_scanner.c", binary_path, compiler_spec)
                            write_text(result_dir / "compile_log.txt", compile_log)
                            if not ok:
                                metrics_json = dict(struct)
                                metrics_json.update({"compile_failed": True, "completed": False, "status": "compile_failed", "error_message": "scanner compilation failed"})
                                perf_payload = {"perf_available": False, "perf_status": "compile_failed"}
                                write_json(result_dir / "metrics.json", metrics_json)
                                write_json(result_dir / "perf_metrics.json", perf_payload)
                                append_condition_rows(metrics_json, perf_payload)
                                continue

                            disassembly = run_cmd(["objdump", "-d", str(binary_path)]).stdout if env["objdump"]["available"] else ""
                            write_text(result_dir / "optional_disassembly.txt", disassembly)
                            struct["classifier_instruction_count"] = parse_instruction_count(disassembly, "classify_semantic") if disassembly else -1
                            struct["transition_instruction_count"] = parse_instruction_count(disassembly, "transition_step") if disassembly else -1
                            struct["compiled_instruction_count"] = struct["classifier_instruction_count"]
                            struct["binary_size_bytes"] = safe_binary_size(binary_path)

                            per_input_rows: List[Dict[str, object]] = []
                            for input_name, _ in eval_inputs:
                                input_path = bench_corpus_dir / "eval" / f"{input_name}.txt"
                                proc = run_cmd([str(binary_path), input_path.as_posix()])
                                if proc.returncode != 0:
                                    raise RuntimeError(f"Binary failed for {binary_path} on {input_path}: {proc.stderr}")
                                metrics = json.loads(proc.stdout)
                                metrics["input_name"] = input_name
                                per_input_rows.append(metrics)
                            write_csv(result_dir / "per_input_metrics.csv", per_input_rows)
                            agg = aggregate_metrics(per_input_rows)
                            perf_payload = {"perf_available": False, "perf_status": "disabled_in_config", "cycles": None, "instructions": None, "branches": None, "branch_misses": None}
                            if config.get("perf", {}).get("enabled", False):
                                first_input = bench_corpus_dir / "eval" / f"{eval_inputs[0][0]}.txt"
                                perf_payload = run_perf(binary_path, first_input)
                                perf_payload.setdefault("cycles", None)
                                perf_payload.setdefault("instructions", None)
                                perf_payload.setdefault("branches", None)
                                perf_payload.setdefault("branch_misses", None)
                            write_json(result_dir / "perf_metrics.json", perf_payload)
                            metrics_json = dict(struct)
                            metrics_json.update(agg)
                            metrics_json.update({
                                "throughput_bytes_per_sec": agg["bytes_per_second_aggregate"],
                                "compile_failed": False,
                                "completed": True,
                                "status": "completed",
                                "error_message": "",
                                "perf_available": perf_payload.get("perf_available", False),
                                "cycles": perf_payload.get("cycles"),
                                "instructions": perf_payload.get("instructions"),
                                "branches": perf_payload.get("branches"),
                                "branch_misses": perf_payload.get("branch_misses"),
                            })
                            write_json(result_dir / "metrics.json", metrics_json)
                            append_condition_rows(metrics_json, perf_payload)

    write_csv(summaries_dir / "benchmark_metadata.csv", benchmark_rows)
    write_csv(summaries_dir / "benchmark_metadata_summary.csv", benchmark_rows)
    write_csv(summaries_dir / "structural_summary.csv", structural_rows)
    write_csv(summaries_dir / "runtime_summary.csv", runtime_rows)
    write_csv(summaries_dir / "code_size_summary.csv", code_size_rows)
    write_csv(summaries_dir / "perf_summary.csv", perf_rows)
    write_csv(summaries_dir / "profile_shift_summary.csv", profile_rows)
    write_csv(summaries_dir / "ablation_summary.csv", ablation_rows)

    compiler_rows = [{
        "benchmark_id": row.get("benchmark_id"),
        "benchmark_family": row.get("benchmark_family"),
        "granularity": row.get("granularity"),
        "semantic_label_mode": row.get("semantic_label_mode"),
        "rollback_split_condition": row.get("rollback_split_condition"),
        "method_id": row.get("method_id"),
        "compiler": row.get("compiler"),
        "compiler_id": row.get("compiler_id"),
        "opt_level": row.get("opt_level"),
        "profile_condition": row.get("profile_condition"),
        "status": row.get("status"),
        "error_message": row.get("error_message"),
        "throughput_bytes_per_sec": row.get("throughput_bytes_per_sec"),
        "compiled_instruction_count": row.get("compiled_instruction_count"),
        "binary_size_bytes": row.get("binary_size_bytes"),
    } for row in structural_rows]
    write_csv(summaries_dir / "compiler_matrix_summary.csv", compiler_rows)
    write_csv(summaries_dir / "merged_condition_summary.csv", structural_rows)

    rollback_delta_rows: List[Dict[str, object]] = []
    keyed = {}
    for row in structural_rows:
        key = (row.get("benchmark_id"), row.get("granularity"), row.get("method_id"), row.get("compiler_id"), row.get("profile_condition"))
        keyed.setdefault(key, {})[row.get("rollback_split_condition")] = row
    for key, pair in keyed.items():
        off = pair.get("off")
        on = pair.get("on")
        if not off or not on or off.get("status") != "completed" or on.get("status") != "completed":
            continue
        rollback_delta_rows.append({
            "benchmark_id": key[0],
            "granularity": key[1],
            "method_id": key[2],
            "compiler_id": key[3],
            "profile_condition": key[4],
            "semantic_mode_off": off.get("semantic_label_mode"),
            "semantic_mode_on": on.get("semantic_label_mode"),
            "delta_num_semantic_classes": (on.get("num_semantic_classes") or 0) - (off.get("num_semantic_classes") or 0),
            "delta_num_intervals_total": (on.get("num_intervals_total") or 0) - (off.get("num_intervals_total") or 0),
            "delta_comparison_sites": (on.get("comparison_sites") or 0) - (off.get("comparison_sites") or 0),
            "delta_worst_case_depth": (on.get("worst_case_depth") or 0) - (off.get("worst_case_depth") or 0),
            "delta_expected_depth": (on.get("expected_depth") or 0.0) - (off.get("expected_depth") or 0.0),
            "delta_accept_predicate_expected_depth": (on.get("accept_predicate_expected_depth") or 0.0) - (off.get("accept_predicate_expected_depth") or 0.0),
            "delta_rollbackacc_predicate_expected_depth": (on.get("rollbackacc_predicate_expected_depth") or 0.0) - (off.get("rollbackacc_predicate_expected_depth") or 0.0),
            "delta_throughput_bytes_per_sec": (on.get("throughput_bytes_per_sec") or 0.0) - (off.get("throughput_bytes_per_sec") or 0.0),
        })
    write_csv(summaries_dir / "rollback_ablation_summary.csv", rollback_delta_rows)
    create_plots(structural_rows, runtime_rows, profile_rows, plots_dir)
    LOGGER.info("Done. Results written to %s", ROOT)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
