#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 14

#define NUM_TOKENS 11

static const char *CLASS_NAMES[14] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0::rb0",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0::rb1",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=EQ::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::VALUE::tok=BARE_VALUE::aux=VALUE::prio0::rb1",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
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

static const int CLASS_ACCEPTING[14] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[14] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    6, 6, 6, 6, 1, 4, 7, 2, 3, 5, 8, 9, 0, 10,
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
    if (s <= 7) return 7;
    return 8;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 9) return 9;
    return 10;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 8) return dag_node_4(s);
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 10) return dag_node_6(s);
    return 11;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 12) return 12;
    return 13;
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 11) return dag_node_7(s);
    return dag_node_8(s);
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 6) return dag_node_3(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 3) return dag_node_1(s);
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_11(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_12(s);
}
