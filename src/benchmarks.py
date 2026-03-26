from __future__ import annotations

import json
import random
from collections import defaultdict
from typing import Dict, Iterable, List, Sequence, Tuple

from core import BenchmarkSpec, DFABuilder


LOWER = "abcdefghijklmnopqrstuvwxyz"
UPPER = LOWER.upper()
LETTERS = LOWER + UPPER
DIGITS = "0123456789"
HEX_DIGITS = DIGITS + "abcdefABCDEF"
IDENT_START = LETTERS + "_"
IDENT_REST = IDENT_START + DIGITS
WS = " \t\r\n"
PRINTABLE_NO_CTRL = "".join(chr(i) for i in range(32, 127))
STRING_SAFE_DOUBLE = "".join(ch for ch in PRINTABLE_NO_CTRL if ch not in '"\\')
STRING_SAFE_SINGLE = "".join(ch for ch in PRINTABLE_NO_CTRL if ch not in "'\\")
STRING_SAFE_JSON = "".join(ch for ch in STRING_SAFE_DOUBLE if ch not in "\b\f\n\r\t")
TEXT_HTML = "".join(ch for ch in PRINTABLE_NO_CTRL if ch not in "<")
VALUE_SAFE = "".join(ch for ch in PRINTABLE_NO_CTRL if ch not in "\r\n=#;[]")


def _is_digit(b: int) -> bool:
    return chr(b) in DIGITS


def _is_ident_start(b: int) -> bool:
    return chr(b) in IDENT_START


def _is_ident_rest(b: int) -> bool:
    return chr(b) in IDENT_REST


def _is_ws(b: int) -> bool:
    return chr(b) in WS


def _is_hex(b: int) -> bool:
    return chr(b) in HEX_DIGITS


def _build_keyword_trie(builder: DFABuilder, keywords: Sequence[str], id_state: str, *, family: str = "KEYWORD", aux_group: str = "KEYWORD_PREFIX") -> Tuple[Dict[str, str], Dict[str, str]]:
    prefix_state: Dict[str, str] = {}
    final_state: Dict[str, str] = {}
    root_children = defaultdict(set)
    for kw in keywords:
        for i in range(1, len(kw) + 1):
            prefix = kw[:i]
            prefix_state[prefix] = f"kw_{prefix}"
            root_children[prefix[:-1]].add(prefix[-1])
    for prefix, state_name in prefix_state.items():
        accepting = prefix in keywords
        builder.add_state(
            state_name,
            accepting=accepting,
            token_kind=(f"KW_{prefix.upper()}" if accepting else None),
            family=(family if accepting else None),
            rollback=accepting,
            priority_sensitive=accepting,
            aux_group=aux_group,
        )
        if accepting:
            final_state[prefix] = state_name
    for prefix, state_name in prefix_state.items():
        for ch in root_children[prefix]:
            builder.add_transition_chars(state_name, ch, prefix_state[prefix + ch])
        builder.add_transition_predicate(state_name, _is_ident_rest, id_state)
    return prefix_state, final_state


def _make_texts(build_a, build_b, *, profile_a: int, profile_b: int, eval_sizes_a: Sequence[int], eval_sizes_b: Sequence[int]) -> Tuple[Dict[str, str], Dict[str, str], Dict[str, List[str]]]:
    text_a = build_a(profile_a)
    text_b = build_b(profile_b)

    def _interleave_uniform(a: str, b: str, chunk: int = 512) -> str:
        parts: List[str] = []
        ia = 0
        ib = 0
        while ia < len(a) or ib < len(b):
            if ia < len(a):
                parts.append(a[ia: ia + chunk])
                ia += chunk
            if ib < len(b):
                parts.append(b[ib: ib + chunk])
                ib += chunk
        return "".join(parts)

    profile_inputs = {
        "A": text_a,
        "B": text_b,
        "uniform": _interleave_uniform(text_a, text_b),
    }
    eval_inputs: Dict[str, str] = {}
    eval_groups: Dict[str, List[str]] = {"A": [], "B": []}
    for idx, size in enumerate(eval_sizes_a, start=1):
        name = f"A_eval_{idx}"
        eval_inputs[name] = build_a(size)
        eval_groups["A"].append(name)
    for idx, size in enumerate(eval_sizes_b, start=1):
        name = f"B_eval_{idx}"
        eval_inputs[name] = build_b(size)
        eval_groups["B"].append(name)
    return profile_inputs, eval_inputs, eval_groups


def build_small_c_benchmark() -> BenchmarkSpec:
    b = DFABuilder("BM1", "Small C-like lexer", notes="Identifiers, integers, simple operators, delimiters, whitespace, dead state")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("id", accepting=True, token_kind="IDENT", family="IDENT", rollback=True, aux_group="WORD")
    b.add_state("int", accepting=True, token_kind="INT", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("eq", accepting=True, token_kind="ASSIGN", family="OPERATOR", rollback=True, aux_group="OP")
    b.add_state("eqeq", accepting=True, token_kind="EQEQ", family="OPERATOR", rollback=False, aux_group="OP")
    for name, tok in [("plus", "PLUS"), ("minus", "MINUS"), ("star", "STAR"), ("slash", "SLASH")]:
        b.add_state(name, accepting=True, token_kind=tok, family="OPERATOR", rollback=False, aux_group="OP")
    b.add_state("ws", accepting=True, token_kind="WS", family="WHITESPACE", rollback=False, aux_group="WS")
    for name, tok in [("lpar", "LPAR"), ("rpar", "RPAR"), ("lbrace", "LBRACE"), ("rbrace", "RBRACE"), ("semi", "SEMI"), ("comma", "COMMA")]:
        b.add_state(name, accepting=True, token_kind=tok, family="DELIM", rollback=False, aux_group="DELIM")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")

    b.add_transition_predicate("start", _is_ident_start, "id")
    b.add_transition_predicate("start", _is_digit, "int")
    b.add_transition_predicate("start", _is_ws, "ws")
    b.add_transition_chars("start", "=", "eq")
    b.add_transition_chars("start", "+", "plus")
    b.add_transition_chars("start", "-", "minus")
    b.add_transition_chars("start", "*", "star")
    b.add_transition_chars("start", "/", "slash")
    b.add_transition_chars("start", "(", "lpar")
    b.add_transition_chars("start", ")", "rpar")
    b.add_transition_chars("start", "{", "lbrace")
    b.add_transition_chars("start", "}", "rbrace")
    b.add_transition_chars("start", ";", "semi")
    b.add_transition_chars("start", ",", "comma")
    b.add_transition_predicate("id", _is_ident_rest, "id")
    b.add_transition_predicate("int", _is_digit, "int")
    b.add_transition_predicate("ws", _is_ws, "ws")
    b.add_transition_chars("eq", "=", "eqeq")

    dfa = b.build()

    ids_a = ["foo", "bar", "alpha", "beta2", "gamma_1", "value", "tmp", "idx"]
    ids_b = ["node", "count", "acc", "cursor", "token", "state", "ptr", "buffer"]
    nums_a = ["0", "1", "2", "10", "42", "314", "9999"]
    nums_b = ["3", "5", "8", "13", "21", "34", "55"]
    ops = ["=", "+", "-", "*", "/", "=="]

    def _builder(seed: int, ids: Sequence[str], nums: Sequence[str], compact: bool):
        rng = random.Random(seed)
        def make_stmt() -> str:
            lhs = rng.choice(ids)
            rhs1 = rng.choice(list(ids) + list(nums))
            rhs2 = rng.choice(list(ids) + list(nums))
            op = rng.choice(ops)
            if compact:
                return f"{lhs}={rhs1}{op}{rhs2};\n" if rng.random() < 0.5 else f"{lhs}={rhs1};\n"
            if rng.random() < 0.35:
                return f"{lhs} = {rhs1} {op} {rhs2};\n"
            if rng.random() < 0.5:
                return f"{lhs} = {rhs1};\n"
            return f"({lhs}) , {rhs1} ;\n"
        def build_text(target_bytes: int) -> str:
            parts: List[str] = ["{\n"]
            while sum(len(p) for p in parts) < target_bytes:
                parts.append(make_stmt())
            parts.append("}\n")
            return "".join(parts)
        return build_text

    build_a = _builder(101, ids_a, nums_a, False)
    build_b = _builder(102, ids_b, nums_b, True)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=80_000, profile_b=70_000, eval_sizes_a=[120_000, 150_000], eval_sizes_b=[100_000, 125_000])
    return BenchmarkSpec("BM1", "Small C-like lexer", dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups)


def build_rich_c_benchmark() -> BenchmarkSpec:
    b = DFABuilder("BM2", "Richer C-like lexer", notes="Keywords, identifiers, ints/floats, operators, strings, line comments, whitespace")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("id", accepting=True, token_kind="IDENT", family="IDENT", rollback=True, aux_group="WORD")
    b.add_state("num_int", accepting=True, token_kind="INT", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("num_dot", accepting=False, aux_group="NUMBER_PREFIX")
    b.add_state("num_frac", accepting=True, token_kind="FLOAT", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("ws", accepting=True, token_kind="WS", family="WHITESPACE", aux_group="WS")
    b.add_state("str_body", accepting=False, aux_group="STRING_BODY")
    b.add_state("str_escape", accepting=False, aux_group="STRING_ESCAPE")
    b.add_state("str_end", accepting=True, token_kind="STRING", family="STRING", aux_group="STRING")
    b.add_state("slash", accepting=True, token_kind="SLASH", family="OPERATOR", rollback=True, aux_group="OP")
    b.add_state("line_comment", accepting=True, token_kind="COMMENT_LINE", family="COMMENT", aux_group="COMMENT")
    op_accepting = {
        "eq": ("ASSIGN", True), "eqeq": ("EQEQ", False), "bang": ("BANG", True), "neq": ("NEQ", False),
        "lt": ("LT", True), "le": ("LE", False), "gt": ("GT", True), "ge": ("GE", False),
        "plus": ("PLUS", True), "plusplus": ("PLUSPLUS", False), "minus": ("MINUS", True), "minusminus": ("MINUSMINUS", False),
        "arrow": ("ARROW", False), "amp": ("AMP", True), "andand": ("ANDAND", False), "pipe": ("PIPE", True), "oror": ("OROR", False),
        "star": ("STAR", False),
    }
    for name, (tok, rollback) in op_accepting.items():
        b.add_state(name, accepting=True, token_kind=tok, family="OPERATOR", rollback=rollback, aux_group="OP")
    for name, tok in [("lpar", "LPAR"), ("rpar", "RPAR"), ("lbrace", "LBRACE"), ("rbrace", "RBRACE"), ("lbrack", "LBRACK"), ("rbrack", "RBRACK"), ("semi", "SEMI"), ("comma", "COMMA")]:
        b.add_state(name, accepting=True, token_kind=tok, family="DELIM", aux_group="DELIM")
    keywords = ["if", "int", "for", "while", "return"]
    prefix_state, _ = _build_keyword_trie(b, keywords, id_state="id")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")

    keyword_starts = {kw[0]: prefix_state[kw[0]] for kw in keywords}
    for ch, st in keyword_starts.items():
        b.add_transition_chars("start", ch, st)
    for ch in IDENT_START:
        if ch not in keyword_starts:
            b.add_transition_chars("start", ch, "id")
    b.add_transition_predicate("start", _is_ws, "ws")
    b.add_transition_predicate("start", _is_digit, "num_int")
    b.add_transition_chars("start", '"', "str_body")
    b.add_transition_chars("start", "/", "slash")
    b.add_transition_chars("start", "=", "eq")
    b.add_transition_chars("start", "!", "bang")
    b.add_transition_chars("start", "<", "lt")
    b.add_transition_chars("start", ">", "gt")
    b.add_transition_chars("start", "+", "plus")
    b.add_transition_chars("start", "-", "minus")
    b.add_transition_chars("start", "*", "star")
    b.add_transition_chars("start", "&", "amp")
    b.add_transition_chars("start", "|", "pipe")
    b.add_transition_chars("start", "(", "lpar")
    b.add_transition_chars("start", ")", "rpar")
    b.add_transition_chars("start", "{", "lbrace")
    b.add_transition_chars("start", "}", "rbrace")
    b.add_transition_chars("start", "[", "lbrack")
    b.add_transition_chars("start", "]", "rbrack")
    b.add_transition_chars("start", ";", "semi")
    b.add_transition_chars("start", ",", "comma")

    b.add_transition_predicate("id", _is_ident_rest, "id")
    b.add_transition_predicate("num_int", _is_digit, "num_int")
    b.add_transition_chars("num_int", ".", "num_dot")
    b.add_transition_predicate("num_dot", _is_digit, "num_frac")
    b.add_transition_predicate("num_frac", _is_digit, "num_frac")
    b.add_transition_predicate("ws", _is_ws, "ws")
    for ch in STRING_SAFE_DOUBLE:
        b.add_transition_chars("str_body", ch, "str_body")
    b.add_transition_chars("str_body", "\\", "str_escape")
    b.add_transition_chars("str_body", '"', "str_end")
    b.add_transition_predicate("str_escape", lambda _: True, "str_body")
    b.add_transition_chars("slash", "/", "line_comment")
    for ch in PRINTABLE_NO_CTRL:
        if ch != "\n":
            b.add_transition_chars("line_comment", ch, "line_comment")
    pairs = {"eq": ("=", "eqeq"), "bang": ("=", "neq"), "lt": ("=", "le"), "gt": ("=", "ge"), "plus": ("+", "plusplus"), "minus": ("-", "minusminus"), "amp": ("&", "andand"), "pipe": ("|", "oror")}
    for src, (ch, dst) in pairs.items():
        b.add_transition_chars(src, ch, dst)
    b.add_transition_chars("minus", ">", "arrow")

    dfa = b.build()
    ids_a = ["value", "count", "idx", "node", "tmp", "buffer", "acc"]
    ids_b = ["cursor", "slot", "entry", "state", "token", "result", "handle"]
    ints = ["0", "1", "2", "10", "42", "100"]
    floats = ["0.5", "1.25", "2.0", "3.14"]
    strs = [json.dumps(s) for s in ["alpha", "beta", "gamma", "delta", "path\\tmp", "a b c"]]
    keywords_pool = keywords

    def _builder(seed: int, ids: Sequence[str], compact: bool):
        rng = random.Random(seed)
        def expr() -> str:
            base = rng.choice(list(ids) + ints + floats)
            if rng.random() < 0.3:
                return base
            op = rng.choice(["+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">="])
            rhs = rng.choice(list(ids) + ints + floats)
            return f"{base}{op}{rhs}" if compact else f"{base} {op} {rhs}"
        def line_comment() -> str:
            words = [rng.choice(["todo", "note", "check", "fastpath", "slowpath", "value"]) for _ in range(4)]
            return "//" + " ".join(words) + "\n" if compact else "// " + " ".join(words) + "\n"
        def stmt() -> str:
            choice = rng.random()
            if choice < 0.2:
                return f"int {rng.choice(ids)} = {rng.choice(ints + floats)};\n"
            if choice < 0.4:
                return f"{rng.choice(ids)} = {expr()};\n"
            if choice < 0.55:
                return f"if ({expr()}) {{ {rng.choice(ids)} = {expr()}; }}\n"
            if choice < 0.7:
                return f"while ({expr()}) {{ {rng.choice(ids)} = {rng.choice(ids)} + 1; }}\n"
            if choice < 0.82:
                return f"return {rng.choice(list(ids) + ints + strs)};\n"
            if choice < 0.9:
                return line_comment()
            kw = rng.choice(keywords_pool)
            return f"{kw} {rng.choice(ids)} ;\n"
        def build_text(target_bytes: int) -> str:
            parts = ["int main(){\n" if compact else "int main() {\n"]
            while sum(len(p) for p in parts) < target_bytes:
                if rng.random() < 0.12:
                    parts.append(f"{rng.choice(ids)} = {rng.choice(strs)};\n")
                parts.append(stmt())
            parts.append("return 0;\n}\n")
            return "".join(parts)
        return build_text

    build_a = _builder(202, ids_a, False)
    build_b = _builder(203, ids_b, True)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=100_000, profile_b=90_000, eval_sizes_a=[140_000, 180_000], eval_sizes_b=[120_000, 160_000])
    return BenchmarkSpec("BM2", "Richer C-like lexer", dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups)


def build_json_like_benchmark() -> BenchmarkSpec:
    b = DFABuilder("BM3", "JSON-like lexer", notes="Structured non-C tokenizer with strings, numbers, booleans, null, punctuation, whitespace")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("ws", accepting=True, token_kind="WS", family="WHITESPACE", aux_group="WS")
    b.add_state("str_body", accepting=False, aux_group="STRING_BODY")
    b.add_state("str_escape", accepting=False, aux_group="STRING_ESCAPE")
    b.add_state("str_end", accepting=True, token_kind="STRING", family="STRING", aux_group="STRING")
    b.add_state("minus", accepting=False, aux_group="NUMBER_PREFIX")
    b.add_state("num_zero", accepting=True, token_kind="NUMBER", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("num_int", accepting=True, token_kind="NUMBER", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("num_dot", accepting=False, aux_group="NUMBER_PREFIX")
    b.add_state("num_frac", accepting=True, token_kind="NUMBER", family="NUMBER", rollback=True, aux_group="NUMBER")
    literals = ["true", "false", "null"]
    prefix_state: Dict[str, str] = {}
    for lit in literals:
        for i in range(1, len(lit) + 1):
            prefix = lit[:i]
            if prefix not in prefix_state:
                accepting = prefix in literals
                b.add_state(f"lit_{prefix}", accepting=accepting, token_kind=(prefix.upper() if accepting else None), family=("LITERAL" if accepting else None), rollback=accepting, priority_sensitive=accepting, aux_group="LITERAL_PREFIX")
                prefix_state[prefix] = f"lit_{prefix}"
    for name, tok in [("lbrace", "LBRACE"), ("rbrace", "RBRACE"), ("lbrack", "LBRACK"), ("rbrack", "RBRACK"), ("colon", "COLON"), ("comma", "COMMA")]:
        b.add_state(name, accepting=True, token_kind=tok, family="PUNCT", aux_group="PUNCT")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")
    b.add_transition_predicate("start", _is_ws, "ws")
    b.add_transition_chars("start", '"', "str_body")
    b.add_transition_chars("start", "-", "minus")
    b.add_transition_chars("start", "0", "num_zero")
    b.add_transition_chars("start", DIGITS[1:], "num_int")
    for ch, st in {"t": prefix_state["t"], "f": prefix_state["f"], "n": prefix_state["n"]}.items():
        b.add_transition_chars("start", ch, st)
    for src, ch, dst in [("start", "{", "lbrace"), ("start", "}", "rbrace"), ("start", "[", "lbrack"), ("start", "]", "rbrack"), ("start", ":", "colon"), ("start", ",", "comma")]:
        b.add_transition_chars(src, ch, dst)
    b.add_transition_predicate("ws", _is_ws, "ws")
    for ch in STRING_SAFE_JSON:
        b.add_transition_chars("str_body", ch, "str_body")
    b.add_transition_chars("str_body", "\\", "str_escape")
    b.add_transition_chars("str_body", '"', "str_end")
    b.add_transition_predicate("str_escape", lambda _: True, "str_body")
    b.add_transition_chars("minus", "0", "num_zero")
    b.add_transition_chars("minus", DIGITS[1:], "num_int")
    b.add_transition_predicate("num_int", _is_digit, "num_int")
    b.add_transition_chars("num_int", ".", "num_dot")
    b.add_transition_chars("num_zero", ".", "num_dot")
    b.add_transition_predicate("num_dot", _is_digit, "num_frac")
    b.add_transition_predicate("num_frac", _is_digit, "num_frac")
    for lit in literals:
        for i in range(1, len(lit)):
            b.add_transition_chars(prefix_state[lit[:i]], lit[i], prefix_state[lit[:i + 1]])

    dfa = b.build()
    rng_a = random.Random(303)
    rng_b = random.Random(304)

    def _build_json_text(rng: random.Random, target_bytes: int, *, pretty: bool) -> str:
        def rand_string() -> str:
            letters = [rng.choice("abcdefxyz0123456789_") for _ in range(rng.randint(3, 12))]
            if rng.random() < 0.15:
                letters.insert(rng.randint(0, len(letters)), '"')
            if rng.random() < 0.15:
                letters.insert(rng.randint(0, len(letters)), '\\')
            return "".join(letters)
        def rand_value(depth: int = 0):
            if depth > 3:
                choice = rng.random()
                if choice < 0.3: return rng.randint(-50, 150)
                if choice < 0.6: return round(rng.uniform(-10.0, 10.0), 2)
                if choice < 0.8: return rng.choice([True, False, None])
                return rand_string()
            choice = rng.random()
            if choice < 0.25: return {f"k{idx}_{rand_string()}": rand_value(depth + 1) for idx in range(rng.randint(1, 4))}
            if choice < 0.45: return [rand_value(depth + 1) for _ in range(rng.randint(1, 5))]
            if choice < 0.62: return rng.randint(-500, 500)
            if choice < 0.78: return round(rng.uniform(-100.0, 100.0), 2)
            if choice < 0.9: return rng.choice([True, False, None])
            return rand_string()
        docs: List[str] = []
        while sum(len(x) for x in docs) < target_bytes:
            obj = rand_value(0)
            docs.append(json.dumps(obj, ensure_ascii=False, separators=(",", ":") if not pretty else None, indent=None if not pretty else 2))
            docs.append("\n")
        return "".join(docs)

    build_a = lambda size: _build_json_text(rng_a, size, pretty=False)
    build_b = lambda size: _build_json_text(rng_b, size, pretty=True)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=90_000, profile_b=80_000, eval_sizes_a=[130_000, 165_000], eval_sizes_b=[110_000, 145_000])
    return BenchmarkSpec("BM3", "JSON-like lexer", dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups)


def build_html_like_benchmark() -> BenchmarkSpec:
    b = DFABuilder("BM4", "HTML-like lexer", notes="HTML/XML-like tokenizer with tags, attributes, strings, text, whitespace")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("ws", accepting=True, token_kind="WS", family="WHITESPACE", aux_group="WS")
    b.add_state("text", accepting=True, token_kind="TEXT", family="TEXT", aux_group="TEXT")
    b.add_state("lt", accepting=True, token_kind="LT", family="TAG_PUNCT", aux_group="TAG")
    b.add_state("lt_slash", accepting=True, token_kind="LT_SLASH", family="TAG_PUNCT", aux_group="TAG")
    b.add_state("gt", accepting=True, token_kind="GT", family="TAG_PUNCT", aux_group="TAG")
    b.add_state("slash_gt", accepting=True, token_kind="SLASH_GT", family="TAG_PUNCT", aux_group="TAG")
    b.add_state("eq", accepting=True, token_kind="EQ", family="TAG_PUNCT", aux_group="TAG")
    b.add_state("name", accepting=True, token_kind="NAME", family="NAME", rollback=True, aux_group="NAME")
    b.add_state("str_body", accepting=False, aux_group="STRING_BODY")
    b.add_state("str_escape", accepting=False, aux_group="STRING_ESCAPE")
    b.add_state("str_end", accepting=True, token_kind="ATTR_STRING", family="STRING", aux_group="STRING")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")
    b.add_transition_predicate("start", _is_ws, "ws")
    b.add_transition_chars("start", "<", "lt")
    b.add_transition_chars("start", ">", "gt")
    b.add_transition_chars("start", "=", "eq")
    b.add_transition_chars("start", '"', "str_body")
    b.add_transition_predicate("start", lambda x: chr(x) in TEXT_HTML and not _is_ws(x), "text")
    b.add_transition_predicate("ws", _is_ws, "ws")
    b.add_transition_predicate("text", lambda x: chr(x) in TEXT_HTML and not _is_ws(x), "text")
    b.add_transition_predicate("lt", _is_ident_start, "name")
    b.add_transition_chars("lt", "/", "lt_slash")
    b.add_transition_predicate("lt_slash", _is_ident_start, "name")
    b.add_transition_chars("lt", ">", "gt")
    b.add_transition_chars("lt", '"', "str_body")
    b.add_transition_chars("lt", "=", "eq")
    b.add_transition_predicate("name", _is_ident_rest, "name")
    b.add_transition_chars("name", "/", "slash_gt")
    b.add_transition_chars("name", '"', "str_body")
    b.add_transition_chars("name", "=", "eq")
    b.add_transition_predicate("name", _is_ws, "ws")
    b.add_transition_chars("name", ">", "gt")
    b.add_transition_chars("slash_gt", ">", "slash_gt")
    b.add_transition_chars("eq", '"', "str_body")
    b.add_transition_predicate("eq", _is_ws, "ws")
    for ch in STRING_SAFE_DOUBLE:
        b.add_transition_chars("str_body", ch, "str_body")
    b.add_transition_chars("str_body", "\\", "str_escape")
    b.add_transition_chars("str_body", '"', "str_end")
    b.add_transition_predicate("str_escape", lambda _: True, "str_body")

    dfa = b.build()

    def _builder(seed: int, *, compact: bool):
        rng = random.Random(seed)
        tags = ["div", "span", "input", "section", "title", "item", "a", "body"]
        attrs = ["class", "id", "href", "data_id", "role", "value", "type"]
        texts = ["hello", "world", "semantic", "layout", "dispatch", "benchmark", "state42"]
        def make_tag() -> str:
            tag = rng.choice(tags)
            pieces = [f"<{tag}"]
            for _ in range(rng.randint(0, 3)):
                attr = rng.choice(attrs)
                val = rng.choice(texts) + (str(rng.randint(0, 50)) if rng.random() < 0.4 else "")
                sep = "" if compact else " "
                pieces.append(f"{sep}{attr}={json.dumps(val)}")
            if rng.random() < 0.25:
                pieces.append("/>")
                return "".join(pieces)
            inner = rng.choice(texts)
            close = f"</{tag}>"
            pieces.append(">" + inner + close)
            return "".join(pieces)
        def build_text(target_bytes: int) -> str:
            parts: List[str] = []
            while sum(len(p) for p in parts) < target_bytes:
                parts.append(make_tag())
                if not compact:
                    parts.append("\n")
            return "".join(parts)
        return build_text

    build_a = _builder(401, compact=False)
    build_b = _builder(402, compact=True)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=85_000, profile_b=70_000, eval_sizes_a=[120_000, 150_000], eval_sizes_b=[95_000, 125_000])
    return BenchmarkSpec("BM4", "HTML-like lexer", dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups)


def build_sql_like_benchmark() -> BenchmarkSpec:
    b = DFABuilder("BM5", "SQL-like lexer", notes="SQL-like tokenizer with keywords, identifiers, numbers, strings, operators, punctuation")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("id", accepting=True, token_kind="IDENT", family="IDENT", rollback=True, aux_group="IDENT")
    b.add_state("num_int", accepting=True, token_kind="INT", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("num_dot", accepting=False, aux_group="NUMBER_PREFIX")
    b.add_state("num_frac", accepting=True, token_kind="FLOAT", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("ws", accepting=True, token_kind="WS", family="WHITESPACE", aux_group="WS")
    b.add_state("sq_body", accepting=False, aux_group="STRING_BODY")
    b.add_state("sq_escape", accepting=False, aux_group="STRING_ESCAPE")
    b.add_state("sq_end", accepting=True, token_kind="STRING", family="STRING", aux_group="STRING")
    for name, tok in [("eq", "EQ"), ("lt", "LT"), ("gt", "GT"), ("le", "LE"), ("ge", "GE"), ("neq", "NEQ"), ("plus", "PLUS"), ("minus", "MINUS"), ("star", "STAR"), ("slash", "SLASH")]:
        b.add_state(name, accepting=True, token_kind=tok, family="OPERATOR", rollback=(tok in {"LT", "GT"}), aux_group="OP")
    for name, tok in [("comma", "COMMA"), ("semi", "SEMI"), ("lpar", "LPAR"), ("rpar", "RPAR"), ("dot", "DOT")]:
        b.add_state(name, accepting=True, token_kind=tok, family="PUNCT", aux_group="PUNCT")
    keywords = ["select", "from", "where", "and", "or", "insert", "into", "values", "update", "set"]
    prefix_state, _ = _build_keyword_trie(b, keywords, id_state="id", family="KEYWORD", aux_group="KEYWORD_PREFIX")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")

    keyword_starts = {kw[0]: prefix_state[kw[0]] for kw in keywords}
    for ch, st in keyword_starts.items():
        b.add_transition_chars("start", ch, st)
    for ch in IDENT_START:
        if ch not in keyword_starts:
            b.add_transition_chars("start", ch, "id")
    b.add_transition_predicate("start", _is_ws, "ws")
    b.add_transition_predicate("start", _is_digit, "num_int")
    b.add_transition_chars("start", "'", "sq_body")
    for src, ch, dst in [("start", "=", "eq"), ("start", "<", "lt"), ("start", ">", "gt"), ("start", "+", "plus"), ("start", "-", "minus"), ("start", "*", "star"), ("start", "/", "slash"), ("start", ",", "comma"), ("start", ";", "semi"), ("start", "(", "lpar"), ("start", ")", "rpar"), ("start", ".", "dot")]:
        b.add_transition_chars(src, ch, dst)
    b.add_transition_predicate("id", _is_ident_rest, "id")
    b.add_transition_predicate("num_int", _is_digit, "num_int")
    b.add_transition_chars("num_int", ".", "num_dot")
    b.add_transition_predicate("num_dot", _is_digit, "num_frac")
    b.add_transition_predicate("num_frac", _is_digit, "num_frac")
    b.add_transition_predicate("ws", _is_ws, "ws")
    for ch in STRING_SAFE_SINGLE:
        b.add_transition_chars("sq_body", ch, "sq_body")
    b.add_transition_chars("sq_body", "'", "sq_end")
    b.add_transition_chars("sq_body", "\\", "sq_escape")
    b.add_transition_predicate("sq_escape", lambda _: True, "sq_body")
    b.add_transition_chars("lt", "=", "le")
    b.add_transition_chars("lt", ">", "neq")
    b.add_transition_chars("gt", "=", "ge")

    dfa = b.build()

    def _builder(seed: int, *, uppercase: bool):
        rng = random.Random(seed)
        ids = ["users", "orders", "inventory", "events", "logs", "state_table", "metrics"]
        cols = ["id", "user_id", "status", "amount", "created_at", "value", "kind"]
        kws = [kw.upper() if uppercase else kw for kw in keywords]
        def qkw(x: str) -> str:
            return x.upper() if uppercase else x
        def stmt() -> str:
            choice = rng.random()
            if choice < 0.35:
                return f"{qkw('select')} {rng.choice(cols)}, {rng.choice(cols)} {qkw('from')} {rng.choice(ids)} {qkw('where')} {rng.choice(cols)} = '{rng.choice(['ok','new','archived'])}';\n"
            if choice < 0.65:
                return f"{qkw('insert')} {qkw('into')} {rng.choice(ids)} ({rng.choice(cols)}, {rng.choice(cols)}) {qkw('values')} ({rng.randint(1,999)}, '{rng.choice(['a','b','c'])}');\n"
            return f"{qkw('update')} {rng.choice(ids)} {qkw('set')} {rng.choice(cols)} = {rng.randint(1,99)} {qkw('where')} {rng.choice(cols)} >= {rng.randint(1,99)};\n"
        def build_text(target_bytes: int) -> str:
            parts: List[str] = []
            while sum(len(p) for p in parts) < target_bytes:
                parts.append(stmt())
            return "".join(parts)
        return build_text

    build_a = _builder(501, uppercase=True)
    build_b = _builder(502, uppercase=False)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=90_000, profile_b=85_000, eval_sizes_a=[125_000, 160_000], eval_sizes_b=[110_000, 145_000])
    return BenchmarkSpec("BM5", "SQL-like lexer", dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups)


def build_python_like_benchmark() -> BenchmarkSpec:
    b = DFABuilder("BM6", "Python-like lexer", notes="Python-like tokenizer with keywords, identifiers, strings, comments, operators, delimiters")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("id", accepting=True, token_kind="IDENT", family="IDENT", rollback=True, aux_group="IDENT")
    b.add_state("num_int", accepting=True, token_kind="INT", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("num_dot", accepting=False, aux_group="NUMBER_PREFIX")
    b.add_state("num_frac", accepting=True, token_kind="FLOAT", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("ws", accepting=True, token_kind="WS", family="WHITESPACE", aux_group="WS")
    b.add_state("sq_body", accepting=False, aux_group="STRING_BODY")
    b.add_state("sq_escape", accepting=False, aux_group="STRING_ESCAPE")
    b.add_state("sq_end", accepting=True, token_kind="STRING", family="STRING", aux_group="STRING")
    b.add_state("dq_body", accepting=False, aux_group="STRING_BODY")
    b.add_state("dq_escape", accepting=False, aux_group="STRING_ESCAPE")
    b.add_state("dq_end", accepting=True, token_kind="STRING", family="STRING", aux_group="STRING")
    b.add_state("hash", accepting=True, token_kind="COMMENT", family="COMMENT", aux_group="COMMENT")
    for name, tok in [("eq", "EQ"), ("eqeq", "EQEQ"), ("colon", "COLON"), ("plus", "PLUS"), ("minus", "MINUS"), ("star", "STAR"), ("slash", "SLASH"), ("comma", "COMMA"), ("dot", "DOT"), ("lpar", "LPAR"), ("rpar", "RPAR"), ("lbrack", "LBRACK"), ("rbrack", "RBRACK")]:
        b.add_state(name, accepting=True, token_kind=tok, family="PUNCT" if tok in {"COLON", "COMMA", "DOT", "LPAR", "RPAR", "LBRACK", "RBRACK"} else "OPERATOR", rollback=(tok == "EQ"), aux_group="PUNCT" if tok in {"COLON", "COMMA", "DOT", "LPAR", "RPAR", "LBRACK", "RBRACK"} else "OP")
    keywords = ["def", "class", "if", "else", "return", "import", "for", "in"]
    prefix_state, _ = _build_keyword_trie(b, keywords, id_state="id", family="KEYWORD", aux_group="KEYWORD_PREFIX")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")
    keyword_starts = {kw[0]: prefix_state[kw[0]] for kw in keywords}
    for ch, st in keyword_starts.items():
        b.add_transition_chars("start", ch, st)
    for ch in IDENT_START:
        if ch not in keyword_starts:
            b.add_transition_chars("start", ch, "id")
    b.add_transition_predicate("start", _is_ws, "ws")
    b.add_transition_predicate("start", _is_digit, "num_int")
    for src, ch, dst in [("start", "'", "sq_body"), ("start", '"', "dq_body"), ("start", "#", "hash"), ("start", "=", "eq"), ("start", ":", "colon"), ("start", "+", "plus"), ("start", "-", "minus"), ("start", "*", "star"), ("start", "/", "slash"), ("start", ",", "comma"), ("start", ".", "dot"), ("start", "(", "lpar"), ("start", ")", "rpar"), ("start", "[", "lbrack"), ("start", "]", "rbrack")]:
        b.add_transition_chars(src, ch, dst)
    b.add_transition_predicate("id", _is_ident_rest, "id")
    b.add_transition_predicate("num_int", _is_digit, "num_int")
    b.add_transition_chars("num_int", ".", "num_dot")
    b.add_transition_predicate("num_dot", _is_digit, "num_frac")
    b.add_transition_predicate("num_frac", _is_digit, "num_frac")
    b.add_transition_predicate("ws", _is_ws, "ws")
    for ch in STRING_SAFE_SINGLE:
        b.add_transition_chars("sq_body", ch, "sq_body")
    b.add_transition_chars("sq_body", "\\", "sq_escape")
    b.add_transition_chars("sq_body", "'", "sq_end")
    b.add_transition_predicate("sq_escape", lambda _: True, "sq_body")
    for ch in STRING_SAFE_DOUBLE:
        b.add_transition_chars("dq_body", ch, "dq_body")
    b.add_transition_chars("dq_body", "\\", "dq_escape")
    b.add_transition_chars("dq_body", '"', "dq_end")
    b.add_transition_predicate("dq_escape", lambda _: True, "dq_body")
    for ch in PRINTABLE_NO_CTRL:
        if ch != "\n":
            b.add_transition_chars("hash", ch, "hash")
    b.add_transition_chars("eq", "=", "eqeq")

    dfa = b.build()
    def _builder(seed: int, *, compact: bool):
        rng = random.Random(seed)
        ids = ["value", "items", "state", "token", "result", "handle", "buffer"]
        modules = ["json", "math", "pathlib", "typing"]
        def line() -> str:
            choice = rng.random()
            if choice < 0.2:
                return f"import {rng.choice(modules)}\n"
            if choice < 0.4:
                return f"def {rng.choice(ids)}({rng.choice(ids)}):\n"
            if choice < 0.6:
                return f"if {rng.choice(ids)} == {rng.randint(0,9)}:\n"
            if choice < 0.75:
                return f"{rng.choice(ids)} = {rng.choice([str(rng.randint(0, 99)), repr(rng.choice(['alpha','beta','gamma']))])}\n"
            if choice < 0.9:
                return f"return {rng.choice(ids)}\n"
            return f"# {rng.choice(['todo', 'fastpath', 'slowpath', 'debug'])} {rng.choice(ids)}\n"
        def build_text(target_bytes: int) -> str:
            parts: List[str] = []
            while sum(len(p) for p in parts) < target_bytes:
                txt = line()
                if compact:
                    txt = txt.replace(" == ", "==")
                parts.append(txt)
            return "".join(parts)
        return build_text

    build_a = _builder(601, compact=False)
    build_b = _builder(602, compact=True)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=80_000, profile_b=70_000, eval_sizes_a=[115_000, 145_000], eval_sizes_b=[95_000, 120_000])
    return BenchmarkSpec("BM6", "Python-like lexer", dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups)


def build_ini_like_benchmark() -> BenchmarkSpec:
    b = DFABuilder("BM7", "INI-like lexer", notes="Configuration/protocol-style tokenizer with sections, keys, values, comments, whitespace")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("ws", accepting=True, token_kind="WS", family="WHITESPACE", aux_group="WS")
    b.add_state("ident", accepting=True, token_kind="IDENT", family="IDENT", rollback=True, aux_group="IDENT")
    b.add_state("value", accepting=True, token_kind="BARE_VALUE", family="VALUE", rollback=True, aux_group="VALUE")
    b.add_state("num", accepting=True, token_kind="NUMBER", family="NUMBER", rollback=True, aux_group="NUMBER")
    b.add_state("str_body", accepting=False, aux_group="STRING_BODY")
    b.add_state("str_escape", accepting=False, aux_group="STRING_ESCAPE")
    b.add_state("str_end", accepting=True, token_kind="STRING", family="STRING", aux_group="STRING")
    b.add_state("comment", accepting=True, token_kind="COMMENT", family="COMMENT", aux_group="COMMENT")
    for name, tok in [("lbrack", "LBRACK"), ("rbrack", "RBRACK"), ("eq", "EQ"), ("dot", "DOT")]:
        b.add_state(name, accepting=True, token_kind=tok, family="PUNCT", aux_group="PUNCT")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")
    b.add_transition_predicate("start", _is_ws, "ws")
    b.add_transition_predicate("start", _is_ident_start, "ident")
    b.add_transition_predicate("start", _is_digit, "num")
    for src, ch, dst in [("start", "[", "lbrack"), ("start", "]", "rbrack"), ("start", "=", "eq"), ("start", ".", "dot"), ("start", '"', "str_body"), ("start", "#", "comment"), ("start", ";", "comment")]:
        b.add_transition_chars(src, ch, dst)
    b.add_transition_predicate("ident", _is_ident_rest, "ident")
    b.add_transition_predicate("num", _is_digit, "num")
    b.add_transition_predicate("ws", _is_ws, "ws")
    b.add_transition_predicate("value", lambda x: chr(x) in VALUE_SAFE, "value")
    b.add_transition_chars("eq", '"', "str_body")
    b.add_transition_predicate("eq", _is_digit, "num")
    b.add_transition_predicate("eq", _is_ident_start, "ident")
    b.add_transition_predicate("eq", lambda x: chr(x) in VALUE_SAFE, "value")
    for ch in STRING_SAFE_DOUBLE:
        b.add_transition_chars("str_body", ch, "str_body")
    b.add_transition_chars("str_body", "\\", "str_escape")
    b.add_transition_chars("str_body", '"', "str_end")
    b.add_transition_predicate("str_escape", lambda _: True, "str_body")
    for ch in PRINTABLE_NO_CTRL:
        if ch != "\n":
            b.add_transition_chars("comment", ch, "comment")

    dfa = b.build()

    def _builder(seed: int, *, spaced: bool):
        rng = random.Random(seed)
        sections = ["server", "database", "logging", "auth", "cache"]
        keys = ["host", "port", "timeout", "enabled", "path", "level", "token"]
        vals = ["true", "false", "debug", "info", "/tmp/data", "localhost", "alpha"]
        def line() -> str:
            choice = rng.random()
            if choice < 0.15:
                return f"[{rng.choice(sections)}]\n"
            if choice < 0.3:
                return f"# {rng.choice(['generated','config','benchmark'])} {rng.choice(keys)}\n"
            sep = " = " if spaced else "="
            rhs = rng.choice(vals + [str(rng.randint(1, 999)), json.dumps(rng.choice(vals))])
            return f"{rng.choice(keys)}{sep}{rhs}\n"
        def build_text(target_bytes: int) -> str:
            parts: List[str] = []
            while sum(len(p) for p in parts) < target_bytes:
                parts.append(line())
            return "".join(parts)
        return build_text

    build_a = _builder(701, spaced=True)
    build_b = _builder(702, spaced=False)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=75_000, profile_b=65_000, eval_sizes_a=[100_000, 130_000], eval_sizes_b=[85_000, 105_000])
    return BenchmarkSpec("BM7", "INI-like lexer", dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups)


def build_synthetic_benchmark(spec_id: str, *, seed: int, token_count: int, prefix_depth: int, class_count: int, profile_skew: float, acceptance_prefix_probability: float, hierarchy_quality: str) -> BenchmarkSpec:
    bench_id = spec_id
    family = f"Synthetic token-trie ({hierarchy_quality})"
    b = DFABuilder(bench_id, family, notes="Synthetic reproducible token-trie benchmark for scaling sweeps")
    b.add_state("start", accepting=False, aux_group="START")
    b.add_state("dead", dead=True, aux_group="DEAD")
    b.set_start("start")
    b.set_dead("dead")
    rng = random.Random(seed)
    alphabet = list("abcdefghijklmnopqrstuvwxyz")
    families = [f"SFAM_{i % max(2, class_count // 3 + 1)}" for i in range(class_count)]

    def synth_token(i: int) -> str:
        prefix = alphabet[i % len(alphabet)] * min(2, prefix_depth)
        core_len = max(2, prefix_depth) + rng.randint(0, 3)
        token = [prefix]
        for _ in range(core_len):
            token.append(rng.choice(alphabet[: max(6, min(20, class_count + 2))]))
        token.append(str(i % 10))
        return "".join(token)

    tokens = [synth_token(i) for i in range(token_count)]
    class_meta: Dict[str, Dict[str, object]] = {}
    for cls_idx in range(class_count):
        family_name = families[cls_idx]
        if hierarchy_quality == "high":
            prio = cls_idx % 2
            rb = (cls_idx // 2) % 2
            aux = f"AUX_{cls_idx % 3}"
        else:
            prio = (cls_idx * 3) % 2
            rb = (cls_idx * 5) % 2
            aux = f"AUX_{cls_idx % 7}"
        class_meta[f"CLASS_{cls_idx}"] = {
            "family": family_name,
            "token_kind": f"TOK_{cls_idx}",
            "priority_sensitive": bool(prio),
            "rollback": bool(rb),
            "aux_group": aux,
        }

    state_names: Dict[str, str] = {"": "start"}
    accepting_prefixes = set()
    for tok_idx, tok in enumerate(tokens):
        cls_name = f"CLASS_{tok_idx % class_count}"
        for i in range(1, len(tok) + 1):
            prefix = tok[:i]
            if prefix in state_names:
                continue
            accepting = i == len(tok) or (i >= 2 and rng.random() < acceptance_prefix_probability)
            meta = class_meta[cls_name] if accepting else {"aux_group": f"PREFIX_{i % 5}"}
            state_name = f"p_{prefix}"
            state_names[prefix] = state_name
            b.add_state(
                state_name,
                accepting=accepting,
                token_kind=meta.get("token_kind") if accepting else None,
                family=meta.get("family") if accepting else None,
                rollback=bool(meta.get("rollback", False)) if accepting else False,
                priority_sensitive=bool(meta.get("priority_sensitive", False)) if accepting else False,
                aux_group=str(meta.get("aux_group", "PREFIX")),
            )
            if accepting:
                accepting_prefixes.add(prefix)
        for i in range(1, len(tok) + 1):
            src = state_names[tok[: i - 1]]
            dst = state_names[tok[:i]]
            b.add_transition_chars(src, tok[i - 1], dst)

    dfa = b.build(metadata={
        "synthetic": True,
        "seed": seed,
        "token_count": token_count,
        "prefix_depth": prefix_depth,
        "class_count": class_count,
        "profile_skew": profile_skew,
        "acceptance_prefix_probability": acceptance_prefix_probability,
        "hierarchy_quality": hierarchy_quality,
        "realized_accepting_prefixes": len(accepting_prefixes),
    })

    weighted_tokens = list(tokens)
    weights = []
    for idx in range(len(tokens)):
        rank = idx + 1
        weights.append(1.0 / (rank ** max(0.1, profile_skew)))
    total_w = sum(weights)
    weights = [w / total_w for w in weights]
    alt_weights = list(reversed(weights))

    def _builder(seed_shift: int, use_alt: bool):
        rg = random.Random(seed + seed_shift)
        probs = alt_weights if use_alt else weights
        def build_text(target_bytes: int) -> str:
            parts: List[str] = []
            while sum(len(p) for p in parts) < target_bytes:
                tok = rg.choices(weighted_tokens, weights=probs, k=1)[0]
                if rg.random() < 0.2:
                    tok += rg.choice([" ", "\n", ",", ";"])
                parts.append(tok)
            return "".join(parts)
        return build_text

    build_a = _builder(11, False)
    build_b = _builder(19, True)
    profile_inputs, eval_inputs, eval_groups = _make_texts(build_a, build_b, profile_a=70_000, profile_b=70_000, eval_sizes_a=[95_000, 120_000], eval_sizes_b=[95_000, 120_000])
    return BenchmarkSpec(bench_id, family, dfa, profile_inputs, eval_inputs, dfa.notes, eval_groups=eval_groups, metadata=dfa.metadata)





def benchmark_builders() -> Dict[str, callable]:
    return {
        "BM1": build_small_c_benchmark,
        "BM2": build_rich_c_benchmark,
        "BM3": build_json_like_benchmark,
        "BM4": build_html_like_benchmark,
        "BM5": build_sql_like_benchmark,
        "BM6": build_python_like_benchmark,
        "BM7": build_ini_like_benchmark,
        "BM_SYN_S": lambda: build_synthetic_benchmark("BM_SYN_S", seed=801, token_count=40, prefix_depth=3, class_count=8, profile_skew=0.8, acceptance_prefix_probability=0.10, hierarchy_quality="high"),
        "BM_SYN_M": lambda: build_synthetic_benchmark("BM_SYN_M", seed=802, token_count=90, prefix_depth=4, class_count=16, profile_skew=1.0, acceptance_prefix_probability=0.14, hierarchy_quality="medium"),
        "BM_SYN_L": lambda: build_synthetic_benchmark("BM_SYN_L", seed=803, token_count=180, prefix_depth=5, class_count=28, profile_skew=1.25, acceptance_prefix_probability=0.18, hierarchy_quality="low"),
    }


def build_selected_benchmarks(ids: Sequence[str]) -> List[BenchmarkSpec]:
    builders = benchmark_builders()
    out: List[BenchmarkSpec] = []
    for bid in ids:
        if bid not in builders:
            raise KeyError(f"Unknown benchmark id: {bid}")
        out.append(builders[bid]())
    return out


def benchmark_registry() -> Dict[str, BenchmarkSpec]:
    return {b.benchmark_id: b for b in build_selected_benchmarks(list(benchmark_builders().keys()))}
