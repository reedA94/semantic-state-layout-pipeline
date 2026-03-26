#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 13

#define DEAD_STATE 0

#define NUM_CLASSES 8

#define NUM_TOKENS 13

static const char *CLASS_NAMES[8] = {
    "DEAD",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATIONAL::rb1",
    "ACC::OPERATIONAL::rb0",
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

static const int CLASS_ACCEPTING[8] = {
    0, 0, 0, 0, 0, 0, 1, 1,
};

static const int CLASS_DEAD[8] = {
    1, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    7, 7, 7, 11, 2, 6, 12, 10, 3, 8, 4, 9, 0, 1,
};

static const int DEAD_OF_STATE[30] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 10) return 1;
    return 2;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 14) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 13) return 3;
    return dag_node_1(s);
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 12) return dag_node_0(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 21) return 6;
    return 7;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 15) return dag_node_3(s);
    return dag_node_4(s);
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_5(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_6(s);
}
