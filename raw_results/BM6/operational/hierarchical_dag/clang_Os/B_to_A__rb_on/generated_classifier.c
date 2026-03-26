#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 23

#define DEAD_STATE 0

#define NUM_CLASSES 8

#define NUM_TOKENS 28

static const char *CLASS_NAMES[8] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATIONAL::rb1",
    "ACC::OPERATIONAL::rb0",
};

static const char *TOKEN_NAMES[28] = {
    "COLON",
    "COMMA",
    "COMMENT",
    "DOT",
    "EQ",
    "EQEQ",
    "FLOAT",
    "IDENT",
    "INT",
    "KW_CLASS",
    "KW_DEF",
    "KW_ELSE",
    "KW_FOR",
    "KW_IF",
    "KW_IMPORT",
    "KW_IN",
    "KW_RETURN",
    "LBRACK",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACK",
    "RPAR",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[8] = {
    0, 0, 0, 0, 0, 0, 1, 1,
};

static const int CLASS_DEAD[8] = {
    1, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 7, 8, 6, 4,
    10, 9, 13, 11, 16, 14, 12, 15, 27, 26, 26, 2, 5, 0, 21, 19,
    25, 24, 1, 3, 18, 23, 17, 22,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 21) return 1;
    return 2;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 25) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 23) return 3;
    return dag_node_1(s);
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 22) return dag_node_0(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 39) return 6;
    return 7;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 27) return dag_node_3(s);
    return dag_node_4(s);
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_5(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_6(s);
}
