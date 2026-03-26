#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 11

#define NUM_TOKENS 11

static const char *CLASS_NAMES[11] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::rb0",
    "ACC::IDENT::rb1",
    "ACC::NUMBER::rb1",
    "ACC::PUNCT::rb0",
    "ACC::STRING::rb0",
    "ACC::VALUE::rb1",
    "ACC::WHITESPACE::rb0",
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

static const int CLASS_ACCEPTING[11] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[11] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    6, 6, 6, 6, 1, 4, 7, 5, 8, 3, 2, 9, 0, 10,
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
    if (s <= 5) return 5;
    return 6;
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 4) return 4;
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 10) return 7;
    return 8;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 12) return 9;
    return 10;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 11) return dag_node_4(s);
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 6) return dag_node_3(s);
    return dag_node_6(s);
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 3) return dag_node_1(s);
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_8(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_9(s);
}
