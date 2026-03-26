#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 13

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 13

#define NUM_TOKENS 10

static const char *CLASS_NAMES[13] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::NAME::tok=NAME::aux=NAME::prio0",
    "ACC::STRING::tok=ATTR_STRING::aux=STRING::prio0",
    "ACC::TAG_PUNCT::tok=EQ::aux=TAG::prio0",
    "ACC::TAG_PUNCT::tok=GT::aux=TAG::prio0",
    "ACC::TAG_PUNCT::tok=LT::aux=TAG::prio0",
    "ACC::TAG_PUNCT::tok=LT_SLASH::aux=TAG::prio0",
    "ACC::TAG_PUNCT::tok=SLASH_GT::aux=TAG::prio0",
    "ACC::TEXT::tok=TEXT::aux=TEXT::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
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

static const int CLASS_ACCEPTING[13] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[13] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[13] = {
    6, 6, 6, 6, 5, 0, 1, 2, 3, 4, 7, 8, 9,
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
    if (s <= 6) return 6;
    return 7;
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 9) return 9;
    return 10;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 8) return 8;
    return dag_node_4(s);
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 7) return dag_node_3(s);
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 11) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 10) return dag_node_6(s);
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 5) return dag_node_2(s);
    return dag_node_8(s);
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 3) return dag_node_1(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_10(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_11(s);
}
