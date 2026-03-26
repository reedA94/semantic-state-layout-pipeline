from __future__ import annotations

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple


@dataclass(frozen=True)
class StateMeta:
    name: str
    accepting: bool = False
    dead: bool = False
    token_kind: Optional[str] = None
    family: Optional[str] = None
    rollback: bool = False
    priority_sensitive: bool = False
    aux_group: str = "GENERIC"


@dataclass
class DFA:
    benchmark_id: str
    family: str
    states: List[StateMeta]
    transitions: List[List[int]]
    start_state: int
    dead_state: int
    notes: str = ""
    metadata: Dict[str, object] = field(default_factory=dict)

    def __post_init__(self) -> None:
        n = len(self.states)
        if len(self.transitions) != n:
            raise ValueError("Transition table/state count mismatch")
        for row in self.transitions:
            if len(row) != 256:
                raise ValueError("Each transition row must contain 256 entries")
            if any((x < 0 or x >= n) for x in row):
                raise ValueError("Transition target out of range")
        if not (0 <= self.start_state < n and 0 <= self.dead_state < n):
            raise ValueError("Invalid start/dead state")
        if not self.states[self.dead_state].dead:
            raise ValueError("dead_state must point to a dead state")

    @property
    def num_states(self) -> int:
        return len(self.states)

    def validate_total_dead(self) -> None:
        dead = self.dead_state
        if any(x != dead for x in self.transitions[dead]):
            raise ValueError("Dead state must self-loop on all bytes")

    def semantic_classes(
        self,
        granularity: str = "fine",
        rollback_split: Optional[str] = None,
        rollback_metadata_available: bool = True,
    ) -> List[str]:
        return [
            semantic_class_for_state(
                s,
                granularity=granularity,
                rollback_split=rollback_split,
                rollback_metadata_available=rollback_metadata_available,
            )
            for s in self.states
        ]


@dataclass
class BenchmarkSpec:
    benchmark_id: str
    family: str
    dfa: DFA
    profile_inputs: Dict[str, str]
    eval_inputs: Dict[str, str]
    notes: str
    eval_groups: Dict[str, List[str]] = field(default_factory=dict)
    metadata: Dict[str, object] = field(default_factory=dict)

    @property
    def profile_input(self) -> str:
        if "A" in self.profile_inputs:
            return self.profile_inputs["A"]
        return next(iter(self.profile_inputs.values()))


@dataclass
class LayoutPlan:
    method_id: str
    method_label: str
    granularity: str
    state_order_old_to_new: List[int]
    new_to_old: List[int]
    class_names: List[str]
    class_of_old_state: List[str]
    class_of_new_state: List[str]
    class_id_of_new_state: List[int]
    class_intervals: Dict[str, List[Tuple[int, int]]]
    classifier_source: str
    classifier_kind: str
    compare_sites: int
    branch_sites: int
    worst_case_depth: int
    depth_by_new_state: List[int]
    metadata: Dict[str, object] = field(default_factory=dict)


@dataclass
class HierarchyNode:
    name: str
    children: List["HierarchyNode"] = field(default_factory=list)
    class_name: Optional[str] = None

    @property
    def is_leaf(self) -> bool:
        return self.class_name is not None


def _normalize_granularity(granularity: str) -> str:
    aliases = {
        "default": "fine",
        "operational_rb_off": "operational",
        "operational_rb_on": "operational",
    }
    return aliases.get(granularity, granularity)


def _normalize_rollback_split(rollback_split: Optional[str]) -> str:
    if rollback_split in {None, "", "auto"}:
        return "auto"
    if rollback_split not in {"off", "on"}:
        raise ValueError(f"Unsupported rollback_split: {rollback_split}")
    return rollback_split


def granularity_implied_rollback_split(granularity: str) -> Optional[str]:
    implied = {
        "operational_rb_off": "off",
        "operational_rb_on": "on",
    }
    return implied.get(granularity)


def effective_rollback_split(
    granularity: str,
    rollback_split: Optional[str] = None,
    rollback_metadata_available: bool = True,
) -> bool:
    normalized_granularity = _normalize_granularity(granularity)
    normalized_split = _normalize_rollback_split(rollback_split)
    if not rollback_metadata_available:
        return False
    if normalized_granularity not in {"coarse", "operational", "fine"}:
        return False
    if normalized_split == "on":
        return True
    if normalized_split == "off":
        return False
    return normalized_granularity == "fine"


def semantic_class_for_state(
    meta: StateMeta,
    granularity: str = "fine",
    rollback_split: Optional[str] = None,
    rollback_metadata_available: bool = True,
) -> str:
    normalized = _normalize_granularity(granularity)
    if normalized not in {"binary", "coarse", "operational", "fine"}:
        raise ValueError(f"Unsupported granularity: {granularity}")

    split_rb = effective_rollback_split(
        normalized,
        rollback_split=rollback_split,
        rollback_metadata_available=rollback_metadata_available,
    )

    if normalized == "binary":
        return "ACCEPTING" if meta.accepting else "NONACC_OR_DEAD"

    if meta.dead:
        return "DEAD"

    if normalized == "coarse":
        if not meta.accepting:
            return f"NONACC::{meta.aux_group or 'GENERIC'}"
        fam = meta.family or "UNKNOWN"
        cls = f"ACC::{fam}"
        if split_rb:
            cls += f"::rb{1 if meta.rollback else 0}"
        return cls

    if normalized == "operational":
        if not meta.accepting:
            return f"NONACC::{meta.aux_group or 'GENERIC'}"
        cls = "ACC::OPERATIONAL"
        if split_rb:
            cls += f"::rb{1 if meta.rollback else 0}"
        return cls

    if not meta.accepting:
        return f"NONACC::{meta.aux_group or 'GENERIC'}"

    fam = meta.family or "UNKNOWN"
    tok = meta.token_kind or "NO_TOKEN"
    aux = meta.aux_group or "GENERIC"
    prio = 1 if meta.priority_sensitive else 0
    cls = f"ACC::{fam}::tok={tok}::aux={aux}::prio{prio}"
    if split_rb:
        cls += f"::rb{1 if meta.rollback else 0}"
    return cls


def class_attributes(class_name: str) -> Dict[str, object]:
    if class_name == "DEAD":
        return {"kind": "dead", "accepting": 0, "rollback": 0}
    if class_name == "NONACC_OR_DEAD":
        return {"kind": "nonacc_or_dead", "accepting": 0, "rollback": 0}
    if class_name == "ACCEPTING":
        return {"kind": "accepting", "accepting": 1, "rollback": 0}
    if class_name.startswith("NONACC::"):
        return {
            "kind": "nonacc",
            "accepting": 0,
            "rollback": 0,
            "aux_group": class_name.split("::", 1)[1],
        }
    if class_name.startswith("ACC::"):
        parts = class_name.split("::")
        out: Dict[str, object] = {
            "kind": "accepting",
            "accepting": 1,
            "family": parts[1] if len(parts) > 1 else "UNKNOWN",
        }
        for p in parts[2:]:
            if p.startswith("tok="):
                out["token_kind"] = p[4:]
            elif p.startswith("aux="):
                out["aux_group"] = p[4:]
            elif p.startswith("prio"):
                out["priority_sensitive"] = int(p.replace("prio", ""))
            elif p.startswith("rb"):
                out["rollback"] = int(p.replace("rb", ""))
        out.setdefault("token_kind", "NO_TOKEN")
        out.setdefault("aux_group", "GENERIC")
        out.setdefault("priority_sensitive", 0)
        out.setdefault("rollback", 0)
        return out
    raise ValueError(f"Cannot parse class name: {class_name}")


class DFABuilder:
    def __init__(self, benchmark_id: str, family: str, notes: str = "") -> None:
        self.benchmark_id = benchmark_id
        self.family = family
        self.notes = notes
        self.states: List[StateMeta] = []
        self.state_index: Dict[str, int] = {}
        self.rows: List[List[int]] = []
        self.dead_state: Optional[int] = None
        self.start_state: Optional[int] = None

    def add_state(self, name: str, **kwargs) -> int:
        if name in self.state_index:
            raise ValueError(f"Duplicate state name: {name}")
        idx = len(self.states)
        self.states.append(StateMeta(name=name, **kwargs))
        self.state_index[name] = idx
        self.rows.append([-1] * 256)
        return idx

    def set_start(self, name: str) -> None:
        self.start_state = self.state_index[name]

    def set_dead(self, name: str) -> None:
        self.dead_state = self.state_index[name]

    def add_transition_bytes(self, src: str, bytes_seq: bytes, dst: str) -> None:
        s = self.state_index[src]
        d = self.state_index[dst]
        for b in bytes_seq:
            self.rows[s][b] = d

    def add_transition_chars(self, src: str, chars: str, dst: str) -> None:
        self.add_transition_bytes(src, chars.encode("latin1"), dst)

    def add_transition_predicate(self, src: str, pred, dst: str) -> None:
        s = self.state_index[src]
        d = self.state_index[dst]
        for b in range(256):
            if pred(b):
                self.rows[s][b] = d

    def fill_missing_to_dead(self) -> None:
        if self.dead_state is None:
            raise ValueError("Dead state not set")
        for row in self.rows:
            for i, x in enumerate(row):
                if x == -1:
                    row[i] = self.dead_state
        for i in range(256):
            self.rows[self.dead_state][i] = self.dead_state

    def build(self, metadata: Optional[Dict[str, object]] = None) -> DFA:
        if self.start_state is None or self.dead_state is None:
            raise ValueError("Start or dead state not set")
        self.fill_missing_to_dead()
        dfa = DFA(
            benchmark_id=self.benchmark_id,
            family=self.family,
            states=self.states,
            transitions=self.rows,
            start_state=self.start_state,
            dead_state=self.dead_state,
            notes=self.notes,
            metadata=metadata or {},
        )
        dfa.validate_total_dead()
        return dfa
