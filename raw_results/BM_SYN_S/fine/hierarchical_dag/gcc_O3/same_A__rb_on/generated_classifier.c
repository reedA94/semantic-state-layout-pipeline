#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 263

#define START_STATE 198

#define DEAD_STATE 0

#define NUM_CLASSES 15

#define NUM_TOKENS 9

static const char *CLASS_NAMES[15] = {
    "DEAD",
    "NONACC::PREFIX_0",
    "NONACC::PREFIX_1",
    "NONACC::PREFIX_2",
    "NONACC::PREFIX_3",
    "NONACC::PREFIX_4",
    "NONACC::START",
    "ACC::SFAM_0::tok=TOK_0::aux=AUX_0::prio0::rb0",
    "ACC::SFAM_0::tok=TOK_3::aux=AUX_0::prio1::rb1",
    "ACC::SFAM_0::tok=TOK_6::aux=AUX_0::prio0::rb1",
    "ACC::SFAM_1::tok=TOK_1::aux=AUX_1::prio1::rb0",
    "ACC::SFAM_1::tok=TOK_4::aux=AUX_1::prio0::rb0",
    "ACC::SFAM_1::tok=TOK_7::aux=AUX_1::prio1::rb1",
    "ACC::SFAM_2::tok=TOK_2::aux=AUX_2::prio0::rb1",
    "ACC::SFAM_2::tok=TOK_5::aux=AUX_2::prio1::rb0",
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

static const int CLASS_ACCEPTING[15] = {
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[15] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7,
    2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    8, 8, 8, 8, 8, 8, 8, 8, 3, 3, 3, 3, 3, 3, 3, 6,
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
    if (s <= 216) return 8;
    return 9;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 207) return 7;
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 239) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 230) return 10;
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 254) return 13;
    return 14;
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 247) return dag_node_8(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 223) return dag_node_6(s);
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 198) return dag_node_4(s);
    return dag_node_11(s);
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_12(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_13(s);
}
