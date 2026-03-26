from __future__ import annotations

import json
from pathlib import Path
from typing import Dict, List, Sequence, Tuple

from core import DFA, LayoutPlan, StateMeta, class_attributes
from layouts import intervals_for_sequence


def permute_dfa(dfa: DFA, layout: LayoutPlan) -> Tuple[List[StateMeta], List[List[int]], int, int]:
    n = dfa.num_states
    new_to_old = layout.new_to_old
    old_to_new = layout.state_order_old_to_new
    new_states = [dfa.states[old_idx] for old_idx in new_to_old]
    new_transitions: List[List[int]] = []
    for new_idx, old_idx in enumerate(new_to_old):
        row = [old_to_new[dfa.transitions[old_idx][b]] for b in range(256)]
        new_transitions.append(row)
    new_start = old_to_new[dfa.start_state]
    new_dead = old_to_new[dfa.dead_state]
    return new_states, new_transitions, new_start, new_dead


def _c_int_array(name: str, values: Sequence[int], c_type: str = "int", cols: int = 16) -> str:
    lines = [f"static const {c_type} {name}[{len(values)}] = {{"]
    for i in range(0, len(values), cols):
        chunk = ", ".join(str(v) for v in values[i : i + cols])
        lines.append(f"    {chunk},")
    lines.append("};")
    return "\n".join(lines)


def _c_string_array(name: str, values: Sequence[str]) -> str:
    escaped = [json.dumps(v) for v in values]
    lines = [f"static const char *{name}[{len(values)}] = {{"]
    for v in escaped:
        lines.append(f"    {v},")
    lines.append("};")
    return "\n".join(lines)


def _c_matrix(name: str, rows: Sequence[Sequence[int]], c_type: str = "unsigned short") -> str:
    lines = [f"static const {c_type} {name}[{len(rows)}][256] = {{"]
    for row in rows:
        chunks = []
        for i in range(0, 256, 16):
            chunks.append(", ".join(str(v) for v in row[i : i + 16]))
        row_text = ",\n        ".join(chunks)
        lines.append("    {")
        lines.append(f"        {row_text}")
        lines.append("    },")
    lines.append("};")
    return "\n".join(lines)


def _class_flags_from_layout(layout: LayoutPlan, new_states: Sequence[StateMeta]) -> Tuple[List[int], List[int]]:
    if layout.method_id == "B2":
        return [0, 1], [0, 0]
    class_accept: Dict[str, int] = {}
    class_dead: Dict[str, int] = {}
    for cls_name in layout.class_names:
        class_accept[cls_name] = 0
        class_dead[cls_name] = 0
    for new_idx, cls_name in enumerate(layout.class_of_new_state):
        st = new_states[new_idx]
        class_accept[cls_name] = max(class_accept[cls_name], 1 if st.accepting else 0)
        class_dead[cls_name] = max(class_dead[cls_name], 1 if st.dead else 0)
    return [class_accept[c] for c in layout.class_names], [class_dead[c] for c in layout.class_names]


def render_classifier_c(layout: LayoutPlan, dfa: DFA) -> str:
    new_states, new_transitions, new_start, new_dead = permute_dfa(dfa, layout)
    class_accepting, class_dead = _class_flags_from_layout(layout, new_states)
    token_names = [st.token_kind or "NO_TOKEN" for st in new_states]
    token_intern = sorted({t for t in token_names})
    token_to_id = {tok: idx for idx, tok in enumerate(token_intern)}
    token_ids = [token_to_id[t] for t in token_names]
    state_dead = [1 if st.dead else 0 for st in new_states]
    class_of_state = layout.class_id_of_new_state

    pieces = [
        "#include <stddef.h>",
        "#include <stdint.h>",
        f"#define NUM_STATES {len(new_states)}",
        f"#define START_STATE {new_start}",
        f"#define DEAD_STATE {new_dead}",
        f"#define NUM_CLASSES {len(layout.class_names)}",
        f"#define NUM_TOKENS {len(token_intern)}",
        _c_string_array("CLASS_NAMES", layout.class_names),
        _c_string_array("TOKEN_NAMES", token_intern),
        _c_int_array("CLASS_ACCEPTING", class_accepting),
        _c_int_array("CLASS_DEAD", class_dead),
        _c_int_array("TOKEN_OF_STATE", token_ids),
        _c_int_array("DEAD_OF_STATE", state_dead),
    ]
    if layout.method_id == "B1":
        pieces.append(_c_int_array("CLASS_OF_STATE", class_of_state))
    if layout.method_id == "B2":
        pieces.append(f"#define ACCEPT_THRESHOLD {layout.metadata['accept_threshold']}")
    pieces.append(layout.classifier_source)
    return "\n\n".join(pieces) + "\n"


def render_scanner_c(layout: LayoutPlan, dfa: DFA) -> str:
    new_states, new_transitions, new_start, new_dead = permute_dfa(dfa, layout)
    class_accepting, class_dead = _class_flags_from_layout(layout, new_states)
    token_names = [st.token_kind or "NO_TOKEN" for st in new_states]
    token_intern = sorted({t for t in token_names})
    token_to_id = {tok: idx for idx, tok in enumerate(token_intern)}
    token_ids = [token_to_id[t] for t in token_names]
    state_dead = [1 if st.dead else 0 for st in new_states]
    class_of_state = layout.class_id_of_new_state
    class_depth = layout.depth_by_new_state

    header = f"""
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STATES {len(new_states)}
#define START_STATE {new_start}
#define DEAD_STATE {new_dead}
#define NUM_CLASSES {len(layout.class_names)}
#define NUM_TOKENS {len(token_intern)}
#define SCAN_REPETITIONS 5
#define TRACE_TARGET_OPS 20000000ULL

static const char *METHOD_ID = {json.dumps(layout.method_id)};
static const char *METHOD_LABEL = {json.dumps(layout.method_label)};
static const char *GRANULARITY = {json.dumps(layout.granularity)};
static const char *BENCHMARK_ID = {json.dumps(dfa.benchmark_id)};
static const char *BENCHMARK_FAMILY = {json.dumps(dfa.family)};
static const int METHOD_REQUIRES_STATE_DEAD_CHECK = {1 if layout.method_id == "B2" else 0};
""".strip()

    arrays = [
        _c_string_array("CLASS_NAMES", layout.class_names),
        _c_string_array("TOKEN_NAMES", token_intern),
        _c_int_array("CLASS_ACCEPTING", class_accepting),
        _c_int_array("CLASS_DEAD", class_dead),
        _c_int_array("TOKEN_OF_STATE", token_ids),
        _c_int_array("DEAD_OF_STATE", state_dead),
        _c_int_array("CLASS_DEPTH_OF_STATE", class_depth),
        _c_matrix("TRANSITIONS", new_transitions),
    ]
    if layout.method_id == "B1":
        arrays.append(_c_int_array("CLASS_OF_STATE", class_of_state))
    if layout.method_id == "B2":
        arrays.append(f"#define ACCEPT_THRESHOLD {layout.metadata['accept_threshold']}")
    arrays.append(layout.classifier_source)

    scanner_impl = r'''
typedef struct {
    unsigned short *trans_states;
    unsigned char *trans_bytes;
    unsigned short *class_states;
    size_t len;
    size_t cap;
} trace_t;

typedef struct {
    uint64_t bytes_processed;
    uint64_t emitted_tokens;
    uint64_t lexical_errors;
    uint64_t transition_calls;
    uint64_t classify_calls;
    uint64_t semantic_comparisons;
    uint64_t avg_depth_numer;
} scan_stats_t;

static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static uint64_t now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

static void trace_init(trace_t *tr) {
    memset(tr, 0, sizeof(*tr));
}

static void trace_free(trace_t *tr) {
    free(tr->trans_states);
    free(tr->trans_bytes);
    free(tr->class_states);
    memset(tr, 0, sizeof(*tr));
}

static void trace_push(trace_t *tr, unsigned short prev_state, unsigned char byte, unsigned short next_state) {
    if (tr == NULL) return;
    if (tr->len == tr->cap) {
        size_t new_cap = tr->cap ? tr->cap * 2 : 4096;
        tr->trans_states = (unsigned short *)realloc(tr->trans_states, new_cap * sizeof(unsigned short));
        tr->trans_bytes = (unsigned char *)realloc(tr->trans_bytes, new_cap * sizeof(unsigned char));
        tr->class_states = (unsigned short *)realloc(tr->class_states, new_cap * sizeof(unsigned short));
        if (!tr->trans_states || !tr->trans_bytes || !tr->class_states) die("Out of memory while growing trace");
        tr->cap = new_cap;
    }
    tr->trans_states[tr->len] = prev_state;
    tr->trans_bytes[tr->len] = byte;
    tr->class_states[tr->len] = next_state;
    tr->len += 1;
}

static unsigned char *read_file(const char *path, size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Cannot open %s: %s\n", path, strerror(errno));
        exit(2);
    }
    if (fseek(f, 0, SEEK_END) != 0) die("fseek failed");
    long size = ftell(f);
    if (size < 0) die("ftell failed");
    rewind(f);
    unsigned char *buf = (unsigned char *)malloc((size_t)size + 1);
    if (!buf) die("Out of memory while reading file");
    size_t got = fread(buf, 1, (size_t)size, f);
    fclose(f);
    if (got != (size_t)size) die("Short read");
    buf[size] = 0;
    *out_size = (size_t)size;
    return buf;
}

static __attribute__((noinline)) unsigned short transition_step(unsigned short s, unsigned char ch) {
    return TRANSITIONS[s][ch];
}

static void scan_buffer(const unsigned char *buf, size_t n, trace_t *trace, scan_stats_t *stats) {
    size_t p = 0;
    memset(stats, 0, sizeof(*stats));
    while (p < n) {
        unsigned short q = START_STATE;
        size_t i = p;
        ptrdiff_t last_accept_pos = -1;
        int last_token = -1;
        while (i < n) {
            unsigned char ch = buf[i];
            unsigned short prev = q;
            q = transition_step(q, ch);
            trace_push(trace, prev, ch, q);
            int cls = classify_semantic(q);
            stats->transition_calls += 1;
            stats->classify_calls += 1;
            stats->semantic_comparisons += (uint64_t)CLASS_DEPTH_OF_STATE[q];
            stats->avg_depth_numer += (uint64_t)CLASS_DEPTH_OF_STATE[q];
            stats->bytes_processed += 1;
            if (CLASS_ACCEPTING[cls]) {
                last_accept_pos = (ptrdiff_t)i;
                last_token = TOKEN_OF_STATE[q];
            }
            if (CLASS_DEAD[cls]) {
                break;
            }
            if (METHOD_REQUIRES_STATE_DEAD_CHECK && DEAD_OF_STATE[q]) {
                break;
            }
            i += 1;
        }
        if (last_accept_pos >= (ptrdiff_t)p) {
            (void)last_token;
            stats->emitted_tokens += 1;
            p = (size_t)last_accept_pos + 1;
        } else {
            stats->lexical_errors += 1;
            p += 1;
        }
    }
}

static uint64_t benchmark_full_scan(const unsigned char *buf, size_t n) {
    uint64_t best = UINT64_MAX;
    scan_stats_t stats;
    for (int rep = 0; rep < SCAN_REPETITIONS; ++rep) {
        uint64_t t0 = now_ns();
        scan_buffer(buf, n, NULL, &stats);
        uint64_t t1 = now_ns();
        if (t1 - t0 < best) best = t1 - t0;
    }
    return best;
}

static uint64_t benchmark_classifier_trace(const trace_t *trace, uint64_t *out_loops) {
    if (trace->len == 0) {
        *out_loops = 0;
        return 0;
    }
    uint64_t loops = TRACE_TARGET_OPS / trace->len;
    if (loops < 1) loops = 1;
    volatile uint64_t sink = 0;
    uint64_t t0 = now_ns();
    for (uint64_t rep = 0; rep < loops; ++rep) {
        for (size_t i = 0; i < trace->len; ++i) {
            sink += (uint64_t)classify_semantic(trace->class_states[i]);
        }
    }
    uint64_t t1 = now_ns();
    if (sink == 123456789ULL) fprintf(stderr, "ignore %" PRIu64 "\n", sink);
    *out_loops = loops;
    return t1 - t0;
}

static uint64_t benchmark_transition_trace(const trace_t *trace, uint64_t *out_loops) {
    if (trace->len == 0) {
        *out_loops = 0;
        return 0;
    }
    uint64_t loops = TRACE_TARGET_OPS / trace->len;
    if (loops < 1) loops = 1;
    volatile uint64_t sink = 0;
    uint64_t t0 = now_ns();
    for (uint64_t rep = 0; rep < loops; ++rep) {
        for (size_t i = 0; i < trace->len; ++i) {
            sink += (uint64_t)transition_step(trace->trans_states[i], trace->trans_bytes[i]);
        }
    }
    uint64_t t1 = now_ns();
    if (sink == 987654321ULL) fprintf(stderr, "ignore %" PRIu64 "\n", sink);
    *out_loops = loops;
    return t1 - t0;
}

static void print_json_escaped(const char *s) {
    putchar('"');
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
        unsigned char c = *p;
        switch (c) {
            case '\\': printf("\\\\"); break;
            case '"': printf("\\\""); break;
            case '\b': printf("\\b"); break;
            case '\f': printf("\\f"); break;
            case '\n': printf("\\n"); break;
            case '\r': printf("\\r"); break;
            case '\t': printf("\\t"); break;
            default:
                if (c < 0x20) {
                    printf("\\u%04x", (unsigned)c);
                } else {
                    putchar((int)c);
                }
        }
    }
    putchar('"');
}

static void print_metrics_json(const char *input_path, size_t n, const trace_t *trace, const scan_stats_t *stats, uint64_t full_ns, uint64_t cls_ns, uint64_t cls_loops, uint64_t trans_ns, uint64_t trans_loops) {
    double cls_ns_per_call = (trace->len && cls_loops) ? ((double)cls_ns / ((double)trace->len * (double)cls_loops)) : 0.0;
    double trans_ns_per_call = (trace->len && trans_loops) ? ((double)trans_ns / ((double)trace->len * (double)trans_loops)) : 0.0;
    double est_cls_share = full_ns ? (cls_ns_per_call * (double)stats->classify_calls) / (double)full_ns : 0.0;
    double est_trans_share = full_ns ? (trans_ns_per_call * (double)stats->transition_calls) / (double)full_ns : 0.0;
    double bytes_per_sec = full_ns ? ((double)n / ((double)full_ns / 1e9)) : 0.0;
    double tokens_per_sec = full_ns ? ((double)stats->emitted_tokens / ((double)full_ns / 1e9)) : 0.0;
    double avg_cmp_per_state = stats->classify_calls ? ((double)stats->semantic_comparisons / (double)stats->classify_calls) : 0.0;
    double avg_cmp_per_token = stats->emitted_tokens ? ((double)stats->semantic_comparisons / (double)stats->emitted_tokens) : 0.0;

    printf("{\n");
    printf("  \"benchmark_id\": \"%s\",\n", BENCHMARK_ID);
    printf("  \"benchmark_family\": \"%s\",\n", BENCHMARK_FAMILY);
    printf("  \"method_id\": \"%s\",\n", METHOD_ID);
    printf("  \"method_label\": \"%s\",\n", METHOD_LABEL);
    printf("  \"granularity\": \"%s\",\n", GRANULARITY);
    printf("  \"input_path\": ");
    print_json_escaped(input_path);
    printf(",\n");
    printf("  \"input_bytes\": %zu,\n", n);
    printf("  \"trace_len\": %zu,\n", trace->len);
    printf("  \"bytes_processed\": %" PRIu64 ",\n", stats->bytes_processed);
    printf("  \"emitted_tokens\": %" PRIu64 ",\n", stats->emitted_tokens);
    printf("  \"lexical_errors\": %" PRIu64 ",\n", stats->lexical_errors);
    printf("  \"transition_calls\": %" PRIu64 ",\n", stats->transition_calls);
    printf("  \"classify_calls\": %" PRIu64 ",\n", stats->classify_calls);
    printf("  \"semantic_comparisons\": %" PRIu64 ",\n", stats->semantic_comparisons);
    printf("  \"avg_semantic_comparisons_per_visited_state\": %.6f,\n", avg_cmp_per_state);
    printf("  \"avg_semantic_comparisons_per_emitted_token\": %.6f,\n", avg_cmp_per_token);
    printf("  \"full_scan_ns\": %" PRIu64 ",\n", full_ns);
    printf("  \"classifier_trace_ns\": %" PRIu64 ",\n", cls_ns);
    printf("  \"classifier_trace_loops\": %" PRIu64 ",\n", cls_loops);
    printf("  \"transition_trace_ns\": %" PRIu64 ",\n", trans_ns);
    printf("  \"transition_trace_loops\": %" PRIu64 ",\n", trans_loops);
    printf("  \"classifier_ns_per_call\": %.12f,\n", cls_ns_per_call);
    printf("  \"transition_ns_per_call\": %.12f,\n", trans_ns_per_call);
    printf("  \"estimated_classification_share_of_full_scan\": %.12f,\n", est_cls_share);
    printf("  \"estimated_transition_share_of_full_scan\": %.12f,\n", est_trans_share);
    printf("  \"bytes_per_second\": %.6f,\n", bytes_per_sec);
    printf("  \"tokens_per_second\": %.6f\n", tokens_per_sec);
    printf("}\n");
}

int main(int argc, char **argv) {
    const char *mode = "default";
    const char *input_path = NULL;
    if (argc == 2) {
        input_path = argv[1];
    } else if (argc == 4 && strcmp(argv[1], "--mode") == 0) {
        mode = argv[2];
        input_path = argv[3];
    } else {
        fprintf(stderr, "Usage: %s <input_file>\n       %s --mode <default|perf_fullscan> <input_file>\n", argv[0], argv[0]);
        return 2;
    }

    size_t n = 0;
    unsigned char *buf = read_file(input_path, &n);

    if (strcmp(mode, "perf_fullscan") == 0) {
        uint64_t full_ns = benchmark_full_scan(buf, n);
        printf("%" PRIu64 "\n", full_ns);
        free(buf);
        return 0;
    }
    if (strcmp(mode, "default") != 0) {
        fprintf(stderr, "Unknown mode: %s\n", mode);
        free(buf);
        return 2;
    }

    trace_t trace;
    trace_init(&trace);
    scan_stats_t stats;
    scan_buffer(buf, n, &trace, &stats);

    uint64_t full_ns = benchmark_full_scan(buf, n);
    uint64_t cls_loops = 0;
    uint64_t trans_loops = 0;
    uint64_t cls_ns = benchmark_classifier_trace(&trace, &cls_loops);
    uint64_t trans_ns = benchmark_transition_trace(&trace, &trans_loops);
    print_metrics_json(input_path, n, &trace, &stats, full_ns, cls_ns, cls_loops, trans_ns, trans_loops);

    trace_free(&trace);
    free(buf);
    return 0;
}
'''
    return header + "\n\n" + "\n\n".join(arrays) + "\n\n" + scanner_impl


def _predicate_interval_metrics(predicate_by_new_state: Sequence[int], visit_counts_by_new_state: Sequence[int] | None = None) -> Dict[str, object]:
    true_intervals = intervals_for_sequence(["T" if x else "F" for x in predicate_by_new_state]).get("T", [])

    def cost_for_state(idx: int) -> int:
        comps = 0
        for lo, hi in true_intervals:
            if lo > 0:
                comps += 1
                if idx < lo:
                    return comps
            if hi < len(predicate_by_new_state) - 1 or lo == 0:
                comps += 1
                if idx <= hi:
                    return comps
        return comps

    comparison_sites = 0
    for lo, hi in true_intervals:
        if lo > 0:
            comparison_sites += 1
        if hi < len(predicate_by_new_state) - 1 or lo == 0:
            comparison_sites += 1
    depth_by_state = [cost_for_state(i) for i in range(len(predicate_by_new_state))]
    total_visits = sum(int(v) for v in (visit_counts_by_new_state or []))
    if total_visits:
        expected_depth = sum(depth_by_state[i] * int(visit_counts_by_new_state[i]) for i in range(len(depth_by_state))) / total_visits
    else:
        expected_depth = (sum(depth_by_state) / len(depth_by_state)) if depth_by_state else 0.0
    return {
        "num_intervals": len(true_intervals),
        "comparison_sites": comparison_sites,
        "worst_case_depth": max(depth_by_state, default=0),
        "expected_depth": expected_depth,
    }


def structural_metrics_for_layout(
    dfa: DFA,
    layout: LayoutPlan,
    *,
    state_visit_counts_old: Sequence[int] | None = None,
    rollback_metadata_available: bool = True,
) -> Dict[str, object]:
    new_states, _, _, _ = permute_dfa(dfa, layout)
    default_intervals = intervals_for_sequence(layout.class_of_new_state)
    semantic_class_count = len(set(layout.class_of_new_state))
    default_total_intervals = sum(len(v) for v in default_intervals.values())
    default_max_intervals = max((len(v) for v in default_intervals.values()), default=0)
    classifier_total_intervals = sum(len(v) for v in layout.class_intervals.values())
    classifier_max_intervals = max((len(v) for v in layout.class_intervals.values()), default=0)
    depth_sum = sum(layout.depth_by_new_state)
    state_visit_counts_old = list(state_visit_counts_old or [0] * dfa.num_states)
    visit_counts_new = [int(state_visit_counts_old[old_idx]) for old_idx in layout.new_to_old]
    total_visits = sum(visit_counts_new)
    expected_depth = (sum(layout.depth_by_new_state[i] * visit_counts_new[i] for i in range(dfa.num_states)) / total_visits) if total_visits else ((depth_sum / dfa.num_states) if dfa.num_states else 0.0)

    accept_predicate = [1 if st.accepting else 0 for st in new_states]
    accept_metrics = _predicate_interval_metrics(accept_predicate, visit_counts_new)

    rollback_counts = {
        "num_accepting_states_total": sum(1 for st in dfa.states if st.accepting),
        "num_accepting_states_rb": sum(1 for st in dfa.states if st.accepting and st.rollback),
        "num_accepting_states_nonrb": sum(1 for st in dfa.states if st.accepting and not st.rollback),
        "num_token_families_with_rb_split": len({
            fam
            for fam in {st.family for st in dfa.states if st.accepting and st.family}
            if any(st.accepting and st.family == fam and st.rollback for st in dfa.states)
            and any(st.accepting and st.family == fam and not st.rollback for st in dfa.states)
        }),
    }

    if rollback_metadata_available:
        rollback_predicate = [1 if (st.accepting and st.rollback) else 0 for st in new_states]
        rollback_metrics = _predicate_interval_metrics(rollback_predicate, visit_counts_new)
    else:
        rollback_metrics = {
            "num_intervals": None,
            "comparison_sites": None,
            "worst_case_depth": None,
            "expected_depth": None,
        }

    out = {
        "benchmark_id": dfa.benchmark_id,
        "benchmark_family": dfa.family,
        "method_id": layout.method_id,
        "method_label": layout.method_label,
        "granularity": layout.granularity,
        "classifier_kind": layout.classifier_kind,
        "num_states": dfa.num_states,
        "num_semantic_classes": len(layout.class_names),
        "semantic_class_count_for_dispatch": len(layout.class_names),
        "semantic_class_count_under_layout": semantic_class_count,
        "default_semantic_total_intervals_in_layout": default_total_intervals,
        "default_semantic_max_intervals_per_class": default_max_intervals,
        "num_intervals_total": classifier_total_intervals,
        "classifier_total_intervals": classifier_total_intervals,
        "max_intervals_per_class": classifier_max_intervals,
        "classifier_max_intervals_per_class": classifier_max_intervals,
        "comparison_sites": layout.compare_sites,
        "compare_sites": layout.compare_sites,
        "branch_sites": layout.branch_sites,
        "leaf_count": len(layout.class_names),
        "worst_case_depth": layout.worst_case_depth,
        "worst_case_dispatch_depth": layout.worst_case_depth,
        "expected_depth": expected_depth,
        "mean_dispatch_depth_over_states": (depth_sum / dfa.num_states) if dfa.num_states else 0.0,
        "comparisons_per_state_visit": expected_depth,
        "layout_metadata": layout.metadata,
        "rollback_metadata_available": rollback_metadata_available,
        "accept_predicate_num_intervals": accept_metrics["num_intervals"],
        "accept_predicate_comparison_sites": accept_metrics["comparison_sites"],
        "accept_predicate_worst_case_depth": accept_metrics["worst_case_depth"],
        "accept_predicate_expected_depth": accept_metrics["expected_depth"],
        "rollbackacc_predicate_available": rollback_metadata_available,
        "rollbackacc_predicate_num_intervals": rollback_metrics["num_intervals"],
        "rollbackacc_predicate_comparison_sites": rollback_metrics["comparison_sites"],
        "rollbackacc_predicate_worst_case_depth": rollback_metrics["worst_case_depth"],
        "rollbackacc_predicate_expected_depth": rollback_metrics["expected_depth"],
        **rollback_counts,
    }
    if "unique_internal_nodes" in layout.metadata:
        out["dag_unique_internal_nodes"] = int(layout.metadata["unique_internal_nodes"])
    if "shared_internal_nodes" in layout.metadata:
        out["dag_shared_internal_nodes"] = int(layout.metadata["shared_internal_nodes"])
    return out
