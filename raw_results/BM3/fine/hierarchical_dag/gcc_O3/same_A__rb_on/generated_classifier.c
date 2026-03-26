#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 13

#define DEAD_STATE 0

#define NUM_CLASSES 18

#define NUM_TOKENS 13

static const char *CLASS_NAMES[18] = {
    "DEAD",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::LITERAL::tok=FALSE::aux=LITERAL_PREFIX::prio1::rb1",
    "ACC::LITERAL::tok=NULL::aux=LITERAL_PREFIX::prio1::rb1",
    "ACC::LITERAL::tok=TRUE::aux=LITERAL_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0::rb1",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACE::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACE::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
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

static const int CLASS_ACCEPTING[18] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static const int CLASS_DEAD[18] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    2, 6, 11, 7, 7, 7, 0, 1, 3, 4, 8, 9, 10, 12,
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
    if (s <= 17) return 7;
    return 8;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 16) return 6;
    return dag_node_4(s);
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 18) return dag_node_5(s);
    return 9;
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 23) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 22) return 10;
    return dag_node_7(s);
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 26) return 14;
    return 15;
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 25) return 13;
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 24) return dag_node_8(s);
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 28) return 16;
    return 17;
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 27) return dag_node_11(s);
    return dag_node_12(s);
}
static __attribute__((noinline)) int dag_node_14(unsigned s) {
    if (s <= 21) return dag_node_6(s);
    return dag_node_13(s);
}
static __attribute__((noinline)) int dag_node_15(unsigned s) {
    if (s <= 15) return dag_node_3(s);
    return dag_node_14(s);
}
static __attribute__((noinline)) int dag_node_16(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_15(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_16(s);
}
