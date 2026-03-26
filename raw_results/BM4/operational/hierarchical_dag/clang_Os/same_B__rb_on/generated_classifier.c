#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 6

#define NUM_TOKENS 10

static const char *CLASS_NAMES[6] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATIONAL::rb1",
    "ACC::OPERATIONAL::rb0",
};

static const char *TOKEN_NAMES[10] = {
    "ATTR_STRING",
    "EQ",
    "GT",
    "LT",
    "LT_SLASH",
    "NAME",
    "NO_TOKEN",
    "SLASH_GT",
    "TEXT",
    "WS",
};

static const int CLASS_ACCEPTING[6] = {
    0, 0, 0, 0, 1, 1,
};

static const int CLASS_DEAD[6] = {
    1, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[13] = {
    6, 6, 6, 6, 5, 9, 8, 3, 4, 2, 7, 1, 0,
};

static const int DEAD_OF_STATE[13] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    if (s <= 4) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 3) return dag_node_1(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_3(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_4(s);
}
