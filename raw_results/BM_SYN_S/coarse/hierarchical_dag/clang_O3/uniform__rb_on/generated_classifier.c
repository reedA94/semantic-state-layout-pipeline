#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 263

#define START_STATE 198

#define DEAD_STATE 0

#define NUM_CLASSES 13

#define NUM_TOKENS 9

static const char *CLASS_NAMES[13] = {
    "DEAD",
    "NONACC::PREFIX_0",
    "NONACC::PREFIX_1",
    "NONACC::PREFIX_2",
    "NONACC::PREFIX_3",
    "NONACC::PREFIX_4",
    "NONACC::START",
    "ACC::SFAM_0::rb1",
    "ACC::SFAM_0::rb0",
    "ACC::SFAM_1::rb1",
    "ACC::SFAM_1::rb0",
    "ACC::SFAM_2::rb1",
    "ACC::SFAM_2::rb0",
};

static const char *TOKEN_NAMES[9] = {
    "NO_TOKEN",
    "TOK_0",
    "TOK_1",
    "TOK_2",
    "TOK_3",
    "TOK_4",
    "TOK_5",
    "TOK_6",
    "TOK_7",
};

static const int CLASS_ACCEPTING[13] = {
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[13] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[263] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4, 4, 7, 7, 4, 4, 4, 7, 4,
    4, 7, 7, 4, 7, 4, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    8, 8, 8, 8, 8, 8, 8, 8, 2, 2, 2, 5, 5, 5, 2, 5,
    2, 5, 5, 5, 2, 5, 2, 5, 3, 3, 3, 3, 3, 3, 3, 6,
    6, 6, 6, 6, 6, 6, 6,
};

static const int DEAD_OF_STATE[263] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 85) return 2;
    return 3;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 35) return 1;
    return dag_node_0(s);
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 197) return 5;
    return 6;
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 164) return 4;
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 121) return dag_node_1(s);
    return dag_node_3(s);
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 214) return 7;
    return 8;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 231) return 9;
    return 10;
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 254) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 247) return dag_node_6(s);
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 223) return dag_node_5(s);
    return dag_node_8(s);
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 198) return dag_node_4(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_10(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_11(s);
}
