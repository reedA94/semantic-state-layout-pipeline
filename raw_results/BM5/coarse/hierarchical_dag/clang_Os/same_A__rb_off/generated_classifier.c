#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 33

#define DEAD_STATE 0

#define NUM_CLASSES 13

#define NUM_TOKENS 31

static const char *CLASS_NAMES[13] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::IDENT",
    "ACC::KEYWORD",
    "ACC::NUMBER",
    "ACC::OPERATOR",
    "ACC::PUNCT",
    "ACC::STRING",
    "ACC::WHITESPACE",
};

static const char *TOKEN_NAMES[31] = {
    "COMMA",
    "DOT",
    "EQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_AND",
    "KW_FROM",
    "KW_INSERT",
    "KW_INTO",
    "KW_OR",
    "KW_SELECT",
    "KW_SET",
    "KW_UPDATE",
    "KW_VALUES",
    "KW_WHERE",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "NEQ",
    "NO_TOKEN",
    "PLUS",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[13] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[13] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 6, 13, 9, 17, 8, 12, 10, 11, 16, 15, 14, 7,
    3, 2, 20, 5, 18, 4, 22, 24, 21, 28, 27, 0, 26, 19, 25, 1,
    29, 30,
};

static const int DEAD_OF_STATE[66] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 31) return 1;
    return 2;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 34) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 33) return 3;
    return dag_node_1(s);
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 32) return dag_node_0(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 46) return 7;
    return 8;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 36) return 6;
    return dag_node_4(s);
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 58) return 9;
    return 10;
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 64) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 63) return dag_node_6(s);
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 48) return dag_node_5(s);
    return dag_node_8(s);
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 35) return dag_node_3(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_10(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_11(s);
}
