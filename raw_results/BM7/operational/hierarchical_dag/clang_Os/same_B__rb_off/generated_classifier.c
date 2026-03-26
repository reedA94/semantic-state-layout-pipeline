#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 5

#define NUM_TOKENS 11

static const char *CLASS_NAMES[5] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATIONAL",
};

static const char *TOKEN_NAMES[11] = {
    "BARE_VALUE",
    "COMMENT",
    "DOT",
    "EQ",
    "IDENT",
    "LBRACK",
    "NO_TOKEN",
    "NUMBER",
    "RBRACK",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[5] = {
    0, 0, 0, 0, 1,
};

static const int CLASS_DEAD[5] = {
    1, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    6, 6, 6, 6, 10, 4, 0, 7, 9, 1, 5, 8, 3, 2,
};

static const int DEAD_OF_STATE[14] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 2) return 2;
    return 3;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 1) return 1;
    return dag_node_0(s);
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 3) return dag_node_1(s);
    return 4;
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_2(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_3(s);
}
