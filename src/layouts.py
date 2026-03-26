from __future__ import annotations

import random
from collections import Counter, defaultdict
from dataclasses import dataclass
from typing import Dict, Iterable, List, Optional, Sequence, Tuple

from core import DFA, HierarchyNode, LayoutPlan, class_attributes


@dataclass(frozen=True)
class BinaryDecisionNode:
    threshold: int
    left: object
    right: object


def stable_unique(seq: Iterable[str]) -> List[str]:
    seen = set()
    out: List[str] = []
    for x in seq:
        if x not in seen:
            out.append(x)
            seen.add(x)
    return out


def apply_permutation(old_to_new: Sequence[int], values: Sequence[object]) -> List[object]:
    out = [None] * len(values)
    for old_idx, new_idx in enumerate(old_to_new):
        out[new_idx] = values[old_idx]
    return out


def invert_permutation(old_to_new: Sequence[int]) -> List[int]:
    new_to_old = [0] * len(old_to_new)
    for old_idx, new_idx in enumerate(old_to_new):
        new_to_old[new_idx] = old_idx
    return new_to_old


def intervals_for_sequence(classes_by_new_state: Sequence[str]) -> Dict[str, List[Tuple[int, int]]]:
    intervals: Dict[str, List[Tuple[int, int]]] = defaultdict(list)
    if not classes_by_new_state:
        return {}
    start = 0
    cur = classes_by_new_state[0]
    for i in range(1, len(classes_by_new_state)):
        if classes_by_new_state[i] != cur:
            intervals[cur].append((start, i - 1))
            start = i
            cur = classes_by_new_state[i]
    intervals[cur].append((start, len(classes_by_new_state) - 1))
    return dict(intervals)


def make_arbitrary_layout(dfa: DFA, class_of_old_state: Sequence[str], granularity: str, seed: int = 0) -> LayoutPlan:
    n = dfa.num_states
    order = list(range(n))
    rng = random.Random(seed)
    rng.shuffle(order)
    old_to_new = [0] * n
    for new_idx, old_idx in enumerate(order):
        old_to_new[old_idx] = new_idx
    return _make_lookup_plan(
        dfa,
        method_id="B1",
        method_label="arbitrary numbering + generic lookup dispatch",
        granularity=granularity,
        old_to_new=old_to_new,
        class_of_old_state=list(class_of_old_state),
        metadata={"baseline_seed": seed, "layout_policy": "fixed_random_permutation"},
    )


def make_binary_threshold_layout(dfa: DFA, class_of_old_state: Sequence[str], granularity: str) -> LayoutPlan:
    nonacc = [i for i, st in enumerate(dfa.states) if not st.accepting]
    acc = [i for i, st in enumerate(dfa.states) if st.accepting]
    order = nonacc + acc
    old_to_new = [0] * dfa.num_states
    for new_idx, old_idx in enumerate(order):
        old_to_new[old_idx] = new_idx
    new_to_old = invert_permutation(old_to_new)
    class_of_new = ["NONACC_OR_DEAD"] * len(nonacc) + ["ACCEPTING"] * len(acc)
    class_names = ["NONACC_OR_DEAD", "ACCEPTING"]
    class_id_of_new = [0 if i < len(nonacc) else 1 for i in range(dfa.num_states)]
    intervals = intervals_for_sequence(class_of_new)
    classifier_source = """
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s >= ACCEPT_THRESHOLD) return 1;
    return 0;
}
""".strip()
    return LayoutPlan(
        method_id="B2",
        method_label="binary final/non-final threshold encoding",
        granularity=granularity,
        state_order_old_to_new=old_to_new,
        new_to_old=new_to_old,
        class_names=class_names,
        class_of_old_state=list(class_of_old_state),
        class_of_new_state=class_of_new,
        class_id_of_new_state=class_id_of_new,
        class_intervals=intervals,
        classifier_source=classifier_source,
        classifier_kind="binary_threshold",
        compare_sites=1,
        branch_sites=1,
        worst_case_depth=1,
        depth_by_new_state=[1] * dfa.num_states,
        metadata={
            "accept_threshold": len(nonacc),
            "layout_policy": "all_nonaccepting_then_accepting",
            "coarse_only": True,
        },
    )


def make_contiguous_interval_layout(
    dfa: DFA,
    class_of_old_state: Sequence[str],
    granularity: str,
    class_order: Optional[Sequence[str]] = None,
    method_id: str = "B3",
    method_label: str = "contiguous multi-class interval encoding",
    metadata: Optional[Dict[str, object]] = None,
) -> LayoutPlan:
    class_of_old_state = list(class_of_old_state)
    unique_classes = stable_unique(class_of_old_state)
    if class_order is None:
        class_order = sorted(unique_classes)
    else:
        class_order = list(class_order)
    missing = [c for c in unique_classes if c not in class_order]
    if missing:
        class_order = list(class_order) + sorted(missing)

    old_to_new = [0] * dfa.num_states
    cursor = 0
    for cls in class_order:
        for old_idx, cls_old in enumerate(class_of_old_state):
            if cls_old == cls:
                old_to_new[old_idx] = cursor
                cursor += 1
    return _make_linear_interval_plan(
        dfa,
        method_id=method_id,
        method_label=method_label,
        granularity=granularity,
        old_to_new=old_to_new,
        class_of_old_state=class_of_old_state,
        class_order=list(class_order),
        metadata=metadata or {},
    )


def make_frequency_aware_layout(
    dfa: DFA,
    class_of_old_state: Sequence[str],
    granularity: str,
    class_visit_counts: Dict[str, int],
    *,
    profile_source: str,
) -> LayoutPlan:
    classes = stable_unique(class_of_old_state)
    ordered = sorted(classes, key=lambda c: (-class_visit_counts.get(c, 0), c))
    return make_contiguous_interval_layout(
        dfa,
        class_of_old_state,
        granularity=granularity,
        class_order=ordered,
        method_id="B4",
        method_label="frequency-aware interval encoding",
        metadata={
            "layout_policy": "class_blocks_sorted_by_profile_frequency",
            "profile_counts": dict(class_visit_counts),
            "profile_source": profile_source,
        },
    )


def build_hierarchy_for_classes(classes: Sequence[str]) -> HierarchyNode:
    root = HierarchyNode("ROOT")
    dead_classes = [c for c in classes if c == "DEAD"]
    bin_nonacc = [c for c in classes if c == "NONACC_OR_DEAD"]
    acc_bin = [c for c in classes if c == "ACCEPTING"]
    nonacc_classes = [c for c in classes if c.startswith("NONACC::")]
    acc_classes = [c for c in classes if c.startswith("ACC::")]

    if dead_classes:
        root.children.append(HierarchyNode("DEAD", [HierarchyNode(f"LEAF::{c}", class_name=c) for c in sorted(dead_classes)]))
    if bin_nonacc:
        root.children.append(HierarchyNode("NONACC_OR_DEAD", [HierarchyNode(f"LEAF::{c}", class_name=c) for c in sorted(bin_nonacc)]))
    if acc_bin:
        root.children.append(HierarchyNode("ACCEPTING", [HierarchyNode(f"LEAF::{c}", class_name=c) for c in sorted(acc_bin)]))

    if nonacc_classes:
        nonacc_root = HierarchyNode("NONACC")
        by_aux: Dict[str, List[str]] = defaultdict(list)
        for cls in nonacc_classes:
            aux = str(class_attributes(cls).get("aux_group", "GENERIC"))
            by_aux[aux].append(cls)
        for aux in sorted(by_aux):
            nonacc_root.children.append(HierarchyNode(aux, [HierarchyNode(f"LEAF::{c}", class_name=c) for c in sorted(by_aux[aux])]))
        root.children.append(nonacc_root)

    if acc_classes:
        acc_root = HierarchyNode("ACCEPTING")
        by_family: Dict[str, List[str]] = defaultdict(list)
        for cls in acc_classes:
            fam = str(class_attributes(cls).get("family", "UNKNOWN"))
            by_family[fam].append(cls)
        for fam in sorted(by_family):
            fam_root = HierarchyNode(fam)
            by_token: Dict[str, List[str]] = defaultdict(list)
            for cls in by_family[fam]:
                tok = str(class_attributes(cls).get("token_kind", "NO_TOKEN"))
                by_token[tok].append(cls)
            for tok in sorted(by_token):
                tok_root = HierarchyNode(tok)
                by_prio: Dict[int, List[str]] = defaultdict(list)
                for cls in by_token[tok]:
                    by_prio[int(class_attributes(cls).get("priority_sensitive", 0))].append(cls)
                for prio in sorted(by_prio, reverse=True):
                    prio_root = HierarchyNode(f"prio{prio}")
                    by_rb: Dict[int, List[str]] = defaultdict(list)
                    for cls in by_prio[prio]:
                        by_rb[int(class_attributes(cls).get("rollback", 0))].append(cls)
                    for rb in sorted(by_rb, reverse=True):
                        leafs = [HierarchyNode(f"LEAF::{c}", class_name=c) for c in sorted(by_rb[rb])]
                        prio_root.children.append(HierarchyNode(f"rb{rb}", leafs))
                    tok_root.children.append(prio_root)
                fam_root.children.append(tok_root)
            acc_root.children.append(fam_root)
        root.children.append(acc_root)
    return root


def _collect_leaf_classes(node: HierarchyNode) -> List[str]:
    if node.is_leaf:
        return [node.class_name or ""]
    out: List[str] = []
    for ch in node.children:
        out.extend(_collect_leaf_classes(ch))
    return out


def _assign_hierarchical_order(node: HierarchyNode, class_members: Dict[str, List[int]], out_old_state_order: List[int]) -> None:
    if node.is_leaf:
        out_old_state_order.extend(class_members[node.class_name or ""])
        return
    for child in node.children:
        _assign_hierarchical_order(child, class_members, out_old_state_order)


def _treeify_ordered_ranges(items: List[Tuple[int, int, object]]) -> object:
    if len(items) == 1:
        return items[0][2]
    mid = len(items) // 2
    left_items = items[:mid]
    right_items = items[mid:]
    threshold = max(x[1] for x in left_items)
    return BinaryDecisionNode(threshold=threshold, left=_treeify_ordered_ranges(left_items), right=_treeify_ordered_ranges(right_items))


def _build_classifier_tree_from_hierarchy(node: HierarchyNode, class_ranges: Dict[str, Tuple[int, int]]) -> object:
    if node.is_leaf:
        return node.class_name
    ordered_children: List[Tuple[int, int, object]] = []
    for child in node.children:
        child_tree = _build_classifier_tree_from_hierarchy(child, class_ranges)
        child_classes = _collect_leaf_classes(child)
        starts = [class_ranges[c][0] for c in child_classes]
        ends = [class_ranges[c][1] for c in child_classes]
        ordered_children.append((min(starts), max(ends), child_tree))
    ordered_children.sort(key=lambda x: x[0])
    return _treeify_ordered_ranges(ordered_children)


def _render_binary_tree_to_c(tree: object, class_to_id: Dict[str, int], indent: str = "    ") -> str:
    lines: List[str] = []

    def emit(node: object, level: int) -> None:
        pad = indent * level
        if isinstance(node, str):
            lines.append(f"{pad}return {class_to_id[node]};")
            return
        assert isinstance(node, BinaryDecisionNode)
        lines.append(f"{pad}if (s <= {node.threshold}) {{")
        emit(node.left, level + 1)
        lines.append(f"{pad}}} else {{")
        emit(node.right, level + 1)
        lines.append(f"{pad}}}")

    emit(tree, 1)
    return "\n".join(lines)


def _tree_signature(node: object) -> object:
    if isinstance(node, str):
        return ("L", node)
    assert isinstance(node, BinaryDecisionNode)
    return ("N", node.threshold, _tree_signature(node.left), _tree_signature(node.right))


def _render_binary_dag_to_c(tree: object, class_to_id: Dict[str, int]) -> Tuple[str, Dict[str, int]]:
    sig_to_name: Dict[object, str] = {}
    ordered_nodes: List[Tuple[object, BinaryDecisionNode]] = []
    use_counts: Counter = Counter()

    def register(node: object) -> object:
        sig = _tree_signature(node)
        use_counts[sig] += 1
        if isinstance(node, BinaryDecisionNode):
            if sig not in sig_to_name:
                register(node.left)
                register(node.right)
                sig_to_name[sig] = f"dag_node_{len(sig_to_name)}"
                ordered_nodes.append((sig, node))
        return sig

    root_sig = register(tree)
    lines: List[str] = []
    for sig, node in ordered_nodes:
        left_sig = _tree_signature(node.left)
        right_sig = _tree_signature(node.right)
        left_expr = f"{sig_to_name[left_sig]}(s)" if left_sig in sig_to_name else str(class_to_id[node.left])
        right_expr = f"{sig_to_name[right_sig]}(s)" if right_sig in sig_to_name else str(class_to_id[node.right])
        lines.append(f"static __attribute__((noinline)) int {sig_to_name[sig]}(unsigned s) {{")
        lines.append(f"    if (s <= {node.threshold}) return {left_expr};")
        lines.append(f"    return {right_expr};")
        lines.append("}")
    if root_sig in sig_to_name:
        lines.append("__attribute__((noinline)) static int classify_semantic(unsigned s) {")
        lines.append(f"    return {sig_to_name[root_sig]}(s);")
        lines.append("}")
    else:
        lines.append("__attribute__((noinline)) static int classify_semantic(unsigned s) {")
        lines.append(f"    return {class_to_id[tree]};")
        lines.append("}")
    shared_internal = sum(1 for sig, count in use_counts.items() if count > 1 and sig in sig_to_name)
    return "\n".join(lines), {"unique_internal_nodes": len(sig_to_name), "shared_internal_nodes": shared_internal, "total_internal_nodes": sum(1 for _sig in use_counts if _sig in sig_to_name)}


def _count_tree_nodes(tree: object) -> Tuple[int, int, int, Dict[str, int]]:
    depth_map: Dict[str, int] = {}

    def walk(node: object, depth: int) -> Tuple[int, int, int]:
        if isinstance(node, str):
            depth_map[node] = depth
            return (0, 0, depth)
        assert isinstance(node, BinaryDecisionNode)
        left_cmp, left_branch, left_depth = walk(node.left, depth + 1)
        right_cmp, right_branch, right_depth = walk(node.right, depth + 1)
        return (1 + left_cmp + right_cmp, 1 + left_branch + right_branch, max(left_depth, right_depth))

    cmp_sites, branch_sites, max_depth = walk(tree, 0)
    return cmp_sites, branch_sites, max_depth, depth_map


def make_hierarchical_tree_layout(dfa: DFA, class_of_old_state: Sequence[str], granularity: str) -> LayoutPlan:
    return _make_hierarchical_layout(dfa, class_of_old_state, granularity, share_dag=False, method_id="B5", method_label="hierarchical comparison-tree encoding")


def make_hierarchical_dag_layout(dfa: DFA, class_of_old_state: Sequence[str], granularity: str) -> LayoutPlan:
    return _make_hierarchical_layout(dfa, class_of_old_state, granularity, share_dag=True, method_id="B5_DAG", method_label="hierarchical comparison-DAG encoding")


def make_hierarchical_layout(dfa: DFA, class_of_old_state: Sequence[str], granularity: str) -> LayoutPlan:
    return make_hierarchical_tree_layout(dfa, class_of_old_state, granularity)


def _make_hierarchical_layout(dfa: DFA, class_of_old_state: Sequence[str], granularity: str, *, share_dag: bool, method_id: str, method_label: str) -> LayoutPlan:
    class_of_old_state = list(class_of_old_state)
    classes = stable_unique(class_of_old_state)
    hierarchy = build_hierarchy_for_classes(classes)
    class_members: Dict[str, List[int]] = defaultdict(list)
    for old_idx, cls in enumerate(class_of_old_state):
        class_members[cls].append(old_idx)
    old_state_order: List[int] = []
    _assign_hierarchical_order(hierarchy, class_members, old_state_order)
    old_to_new = [0] * dfa.num_states
    for new_idx, old_idx in enumerate(old_state_order):
        old_to_new[old_idx] = new_idx
    new_to_old = invert_permutation(old_to_new)
    class_of_new = apply_permutation(old_to_new, class_of_old_state)
    class_intervals = intervals_for_sequence(class_of_new)
    class_ranges = {c: ranges[0] for c, ranges in class_intervals.items()}
    tree = _build_classifier_tree_from_hierarchy(hierarchy, class_ranges)
    class_names = sorted(class_intervals.keys(), key=lambda c: class_ranges[c][0])
    class_to_id = {c: idx for idx, c in enumerate(class_names)}
    class_id_of_new = [class_to_id[c] for c in class_of_new]
    cmp_sites, branch_sites, max_depth, depth_by_class = _count_tree_nodes(tree)
    depth_by_new_state = [depth_by_class[c] for c in class_of_new]
    metadata: Dict[str, object] = {"hierarchy": hierarchy_to_dict(hierarchy), "layout_policy": "hierarchical_nested_blocks", "dag_sharing": share_dag}
    if share_dag:
        classifier_source, dag_meta = _render_binary_dag_to_c(tree, class_to_id)
        metadata.update(dag_meta)
        classifier_kind = "hierarchical_dag"
    else:
        tree_src_body = _render_binary_tree_to_c(tree, class_to_id)
        classifier_source = "__attribute__((noinline)) static int classify_semantic(unsigned s) {\n" + tree_src_body + "\n}\n"
        classifier_kind = "hierarchical_tree"
    return LayoutPlan(
        method_id=method_id,
        method_label=method_label,
        granularity=granularity,
        state_order_old_to_new=old_to_new,
        new_to_old=new_to_old,
        class_names=class_names,
        class_of_old_state=class_of_old_state,
        class_of_new_state=class_of_new,
        class_id_of_new_state=class_id_of_new,
        class_intervals=class_intervals,
        classifier_source=classifier_source,
        classifier_kind=classifier_kind,
        compare_sites=cmp_sites,
        branch_sites=branch_sites,
        worst_case_depth=max_depth,
        depth_by_new_state=depth_by_new_state,
        metadata=metadata,
    )


def hierarchy_to_dict(node: HierarchyNode) -> Dict[str, object]:
    if node.is_leaf:
        return {"name": node.name, "class_name": node.class_name}
    return {"name": node.name, "children": [hierarchy_to_dict(ch) for ch in node.children]}


def _make_lookup_plan(
    dfa: DFA,
    method_id: str,
    method_label: str,
    granularity: str,
    old_to_new: Sequence[int],
    class_of_old_state: Sequence[str],
    metadata: Optional[Dict[str, object]] = None,
) -> LayoutPlan:
    new_to_old = invert_permutation(old_to_new)
    class_of_new = apply_permutation(old_to_new, class_of_old_state)
    class_names = stable_unique(class_of_new)
    class_to_id = {c: i for i, c in enumerate(class_names)}
    class_id_of_new = [class_to_id[c] for c in class_of_new]
    intervals = intervals_for_sequence(class_of_new)
    classifier_source = """
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
""".strip()
    return LayoutPlan(
        method_id=method_id,
        method_label=method_label,
        granularity=granularity,
        state_order_old_to_new=list(old_to_new),
        new_to_old=new_to_old,
        class_names=class_names,
        class_of_old_state=list(class_of_old_state),
        class_of_new_state=class_of_new,
        class_id_of_new_state=class_id_of_new,
        class_intervals=intervals,
        classifier_source=classifier_source,
        classifier_kind="lookup_table",
        compare_sites=0,
        branch_sites=0,
        worst_case_depth=0,
        depth_by_new_state=[0] * dfa.num_states,
        metadata=metadata or {},
    )


def _make_linear_interval_plan(
    dfa: DFA,
    method_id: str,
    method_label: str,
    granularity: str,
    old_to_new: Sequence[int],
    class_of_old_state: Sequence[str],
    class_order: Sequence[str],
    metadata: Optional[Dict[str, object]] = None,
) -> LayoutPlan:
    new_to_old = invert_permutation(old_to_new)
    class_of_new = apply_permutation(old_to_new, class_of_old_state)
    intervals = intervals_for_sequence(class_of_new)
    ordered_classes = [c for c in class_order if c in intervals]
    class_to_id = {c: i for i, c in enumerate(ordered_classes)}
    class_id_of_new = [class_to_id[c] for c in class_of_new]
    lines = ["__attribute__((noinline)) static int classify_semantic(unsigned s) {"]
    depth_by_class: Dict[str, int] = {}
    compare_sites = max(0, len(ordered_classes) - 1)
    branch_sites = compare_sites
    for idx, cls in enumerate(ordered_classes[:-1]):
        upper = intervals[cls][-1][1]
        lines.append(f"    if (s <= {upper}) return {class_to_id[cls]};")
        depth_by_class[cls] = idx + 1
    last_cls = ordered_classes[-1]
    depth_by_class[last_cls] = compare_sites
    lines.append(f"    return {class_to_id[last_cls]};")
    lines.append("}")
    classifier_source = "\n".join(lines)
    depth_by_new_state = [depth_by_class[c] for c in class_of_new]
    return LayoutPlan(
        method_id=method_id,
        method_label=method_label,
        granularity=granularity,
        state_order_old_to_new=list(old_to_new),
        new_to_old=new_to_old,
        class_names=list(ordered_classes),
        class_of_old_state=list(class_of_old_state),
        class_of_new_state=class_of_new,
        class_id_of_new_state=class_id_of_new,
        class_intervals=intervals,
        classifier_source=classifier_source,
        classifier_kind="linear_threshold_chain",
        compare_sites=compare_sites,
        branch_sites=branch_sites,
        worst_case_depth=compare_sites,
        depth_by_new_state=depth_by_new_state,
        metadata=metadata or {},
    )


def compute_class_visit_counts(class_of_old_state: Sequence[str], state_visits: Sequence[int]) -> Dict[str, int]:
    out = Counter()
    for s in state_visits:
        out[class_of_old_state[s]] += 1
    return dict(out)
