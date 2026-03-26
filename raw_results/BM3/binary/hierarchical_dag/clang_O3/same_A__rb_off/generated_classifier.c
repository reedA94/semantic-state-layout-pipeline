#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 0

#define DEAD_STATE 15

#define NUM_CLASSES 2

#define NUM_TOKENS 13

static const char *CLASS_NAMES[2] = {
    "NONACC_OR_DEAD",
    "ACCEPTING",
};

static const char *TOKEN_NAMES[13] = {
    "COLON",
    "COMMA",
    "FALSE",
    "LBRACE",
    "LBRACK",
    "NO_TOKEN",
    "NULL",
    "NUMBER",
    "RBRACE",
    "RBRACK",
    "STRING",
    "TRUE",
    "WS",
};

static const int CLASS_ACCEPTING[2] = {
    0, 1,
};

static const int CLASS_DEAD[2] = {
    1, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    12, 10, 7, 7, 7, 11, 2, 6, 3, 8, 4, 9, 0, 1,
};

static const int DEAD_OF_STATE[30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 15) return 0;
    return 1;
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_0(s);
}
