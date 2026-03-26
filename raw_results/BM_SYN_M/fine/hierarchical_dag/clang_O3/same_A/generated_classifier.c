#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 621

#define START_STATE 449

#define DEAD_STATE 0

#define NUM_CLASSES 23

#define NUM_TOKENS 17

static const char *CLASS_NAMES[23] = {
    "DEAD",
    "NONACC::PREFIX_0",
    "NONACC::PREFIX_1",
    "NONACC::PREFIX_2",
    "NONACC::PREFIX_3",
    "NONACC::PREFIX_4",
    "NONACC::START",
    "ACC::SFAM_0::tok=TOK_0::aux=AUX_0::prio0::rb0",
    "ACC::SFAM_0::tok=TOK_12::aux=AUX_5::prio0::rb0",
    "ACC::SFAM_0::tok=TOK_6::aux=AUX_6::prio0::rb0",
    "ACC::SFAM_1::tok=TOK_1::aux=AUX_1::prio1::rb1",
    "ACC::SFAM_1::tok=TOK_13::aux=AUX_6::prio1::rb1",
    "ACC::SFAM_1::tok=TOK_7::aux=AUX_0::prio1::rb1",
    "ACC::SFAM_2::tok=TOK_14::aux=AUX_0::prio0::rb0",
    "ACC::SFAM_2::tok=TOK_2::aux=AUX_2::prio0::rb0",
    "ACC::SFAM_2::tok=TOK_8::aux=AUX_1::prio0::rb0",
    "ACC::SFAM_3::tok=TOK_15::aux=AUX_1::prio1::rb1",
    "ACC::SFAM_3::tok=TOK_3::aux=AUX_3::prio1::rb1",
    "ACC::SFAM_3::tok=TOK_9::aux=AUX_2::prio1::rb1",
    "ACC::SFAM_4::tok=TOK_10::aux=AUX_3::prio0::rb0",
    "ACC::SFAM_4::tok=TOK_4::aux=AUX_4::prio0::rb0",
    "ACC::SFAM_5::tok=TOK_11::aux=AUX_4::prio1::rb1",
    "ACC::SFAM_5::tok=TOK_5::aux=AUX_5::prio1::rb1",
};

static const char *TOKEN_NAMES[17] = {
    "NO_TOKEN",
    "TOK_0",
    "TOK_1",
    "TOK_10",
    "TOK_11",
    "TOK_12",
    "TOK_13",
    "TOK_14",
    "TOK_15",
    "TOK_2",
    "TOK_3",
    "TOK_4",
    "TOK_5",
    "TOK_6",
    "TOK_7",
    "TOK_8",
    "TOK_9",
};

static const int CLASS_ACCEPTING[23] = {
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[23] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[621] = {
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
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 8, 8, 8, 8, 8, 8, 8, 8, 8, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
};

static const int DEAD_OF_STATE[621] = {
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
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 176) return 2;
    return 3;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 74) return 1;
    return dag_node_0(s);
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 448) return 5;
    return 6;
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 355) return 4;
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 248) return dag_node_1(s);
    return dag_node_3(s);
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 467) return 8;
    return 9;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 458) return 7;
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 499) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 490) return 10;
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 533) return 14;
    return 15;
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 520) return 13;
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 510) return dag_node_8(s);
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 479) return dag_node_6(s);
    return dag_node_11(s);
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 567) return 17;
    return 18;
}
static __attribute__((noinline)) int dag_node_14(unsigned s) {
    if (s <= 557) return 16;
    return dag_node_13(s);
}
static __attribute__((noinline)) int dag_node_15(unsigned s) {
    if (s <= 590) return 19;
    return 20;
}
static __attribute__((noinline)) int dag_node_16(unsigned s) {
    if (s <= 610) return 21;
    return 22;
}
static __attribute__((noinline)) int dag_node_17(unsigned s) {
    if (s <= 601) return dag_node_15(s);
    return dag_node_16(s);
}
static __attribute__((noinline)) int dag_node_18(unsigned s) {
    if (s <= 579) return dag_node_14(s);
    return dag_node_17(s);
}
static __attribute__((noinline)) int dag_node_19(unsigned s) {
    if (s <= 548) return dag_node_12(s);
    return dag_node_18(s);
}
static __attribute__((noinline)) int dag_node_20(unsigned s) {
    if (s <= 449) return dag_node_4(s);
    return dag_node_19(s);
}
static __attribute__((noinline)) int dag_node_21(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_20(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_21(s);
}
