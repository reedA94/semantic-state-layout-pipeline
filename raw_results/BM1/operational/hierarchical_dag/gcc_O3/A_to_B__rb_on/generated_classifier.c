#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 4

#define NUM_TOKENS 16

static const char *CLASS_NAMES[4] = {
    "DEAD",
    "NONACC::START",
    "ACC::OPERATIONAL::rb1",
    "ACC::OPERATIONAL::rb0",
};

static const char *TOKEN_NAMES[16] = {
    "ASSIGN",
    "COMMA",
    "EQEQ",
    "IDENT",
    "INT",
    "LBRACE",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACE",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "WS",
};

static const int CLASS_ACCEPTING[4] = {
    0, 0, 1, 1,
};

static const int CLASS_DEAD[4] = {
    1, 0, 0, 0,
};

static const int TOKEN_OF_STATE[17] = {
    8, 8, 3, 4, 0, 2, 9, 7, 14, 13, 15, 6, 11, 5, 10, 12,
    1,
};

static const int DEAD_OF_STATE[17] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 4) return 2;
    return 3;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 1) return 1;
    return dag_node_0(s);
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_1(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_2(s);
}
