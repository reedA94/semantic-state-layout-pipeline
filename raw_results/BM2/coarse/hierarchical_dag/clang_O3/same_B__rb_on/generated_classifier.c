#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 15

#define DEAD_STATE 0

#define NUM_CLASSES 15

#define NUM_TOKENS 39

static const char *CLASS_NAMES[15] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::rb0",
    "ACC::DELIM::rb0",
    "ACC::IDENT::rb1",
    "ACC::KEYWORD::rb1",
    "ACC::NUMBER::rb1",
    "ACC::OPERATOR::rb1",
    "ACC::OPERATOR::rb0",
    "ACC::STRING::rb0",
    "ACC::WHITESPACE::rb0",
};

static const char *TOKEN_NAMES[39] = {
    "AMP",
    "ANDAND",
    "ARROW",
    "ASSIGN",
    "BANG",
    "COMMA",
    "COMMENT_LINE",
    "EQEQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_FOR",
    "KW_IF",
    "KW_INT",
    "KW_RETURN",
    "KW_WHILE",
    "LBRACE",
    "LBRACK",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "MINUSMINUS",
    "NEQ",
    "NO_TOKEN",
    "OROR",
    "PIPE",
    "PLUS",
    "PLUSPLUS",
    "RBRACE",
    "RBRACK",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[15] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[15] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 6, 21, 33, 18, 31, 19, 32, 34, 5, 11, 14, 15, 13, 17,
    16, 12, 8, 35, 3, 4, 22, 10, 29, 23, 0, 28, 7, 25, 20, 9,
    30, 24, 2, 1, 27, 36, 37, 38,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 13) return 1;
    return 2;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 16) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 15) return 3;
    return dag_node_1(s);
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 14) return dag_node_0(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 18) return 6;
    return 7;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 27) return 8;
    return 9;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 26) return dag_node_4(s);
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 43) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 34) return 10;
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 54) return 13;
    return 14;
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 53) return dag_node_8(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 32) return dag_node_6(s);
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 17) return dag_node_3(s);
    return dag_node_11(s);
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_12(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_13(s);
}
