#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 17

#define NUM_TOKENS 16

static const char *CLASS_NAMES[17] = {
    "DEAD",
    "NONACC::START",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0::rb0",
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0::rb1",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
};

static const char *TOKEN_NAMES[16] = {
    "ASSIGN",
    "COMMA",
    "EQEQ",
    "IDENT",
    "INT",
    "LBRACE",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACE",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "WS",
};

static const int CLASS_ACCEPTING[17] = {
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static const int CLASS_DEAD[17] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[17] = {
    8, 8, 1, 5, 6, 10, 11, 12, 3, 4, 0, 2, 7, 9, 13, 14,
    15,
};

static const int DEAD_OF_STATE[17] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 3) return 3;
    return 4;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 2) return 2;
    return dag_node_0(s);
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 6) return 6;
    return 7;
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 5) return 5;
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 4) return dag_node_1(s);
    return dag_node_3(s);
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 7) return dag_node_4(s);
    return 8;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 11) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 10) return 10;
    return dag_node_6(s);
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 14) return 14;
    return 15;
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 13) return 13;
    return dag_node_8(s);
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 12) return dag_node_7(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 15) return dag_node_10(s);
    return 16;
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 9) return 9;
    return dag_node_11(s);
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 8) return dag_node_5(s);
    return dag_node_12(s);
}
static __attribute__((noinline)) int dag_node_14(unsigned s) {
    if (s <= 1) return 1;
    return dag_node_13(s);
}
static __attribute__((noinline)) int dag_node_15(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_14(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_15(s);
}
