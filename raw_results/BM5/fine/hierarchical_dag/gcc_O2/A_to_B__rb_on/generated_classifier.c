#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 33

#define DEAD_STATE 0

#define NUM_CLASSES 36

#define NUM_TOKENS 31

static const char *CLASS_NAMES[36] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::KEYWORD::tok=KW_AND::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_FROM::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_INSERT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_INTO::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_OR::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_SELECT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_SET::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_UPDATE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_VALUES::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_WHERE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=SEMI::aux=PUNCT::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
};

static const char *TOKEN_NAMES[31] = {
    "COMMA",
    "DOT",
    "EQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_AND",
    "KW_FROM",
    "KW_INSERT",
    "KW_INTO",
    "KW_OR",
    "KW_SELECT",
    "KW_SET",
    "KW_UPDATE",
    "KW_VALUES",
    "KW_WHERE",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "NEQ",
    "NO_TOKEN",
    "PLUS",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[36] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static const int CLASS_DEAD[36] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 3,
    7, 2, 4, 5, 18, 20, 21, 22, 24, 27, 28, 0, 1, 19, 25, 26,
    29, 30,
};

static const int DEAD_OF_STATE[66] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 31) return 1;
    return 2;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 34) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 33) return 3;
    return dag_node_1(s);
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 32) return dag_node_0(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 37) return 7;
    return 8;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 40) return 10;
    return 11;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 39) return 9;
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 38) return dag_node_4(s);
    return dag_node_6(s);
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 42) return 12;
    return 13;
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 45) return 15;
    return 16;
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 44) return 14;
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 43) return dag_node_8(s);
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 41) return dag_node_7(s);
    return dag_node_11(s);
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 47) return 17;
    return 18;
}
static __attribute__((noinline)) int dag_node_14(unsigned s) {
    if (s <= 46) return dag_node_12(s);
    return dag_node_13(s);
}
static __attribute__((noinline)) int dag_node_15(unsigned s) {
    if (s <= 36) return 6;
    return dag_node_14(s);
}
static __attribute__((noinline)) int dag_node_16(unsigned s) {
    if (s <= 49) return 19;
    return 20;
}
static __attribute__((noinline)) int dag_node_17(unsigned s) {
    if (s <= 52) return 22;
    return 23;
}
static __attribute__((noinline)) int dag_node_18(unsigned s) {
    if (s <= 51) return 21;
    return dag_node_17(s);
}
static __attribute__((noinline)) int dag_node_19(unsigned s) {
    if (s <= 50) return dag_node_16(s);
    return dag_node_18(s);
}
static __attribute__((noinline)) int dag_node_20(unsigned s) {
    if (s <= 54) return 24;
    return 25;
}
static __attribute__((noinline)) int dag_node_21(unsigned s) {
    if (s <= 57) return 27;
    return 28;
}
static __attribute__((noinline)) int dag_node_22(unsigned s) {
    if (s <= 56) return 26;
    return dag_node_21(s);
}
static __attribute__((noinline)) int dag_node_23(unsigned s) {
    if (s <= 55) return dag_node_20(s);
    return dag_node_22(s);
}
static __attribute__((noinline)) int dag_node_24(unsigned s) {
    if (s <= 53) return dag_node_19(s);
    return dag_node_23(s);
}
static __attribute__((noinline)) int dag_node_25(unsigned s) {
    if (s <= 59) return 29;
    return 30;
}
static __attribute__((noinline)) int dag_node_26(unsigned s) {
    if (s <= 62) return 32;
    return 33;
}
static __attribute__((noinline)) int dag_node_27(unsigned s) {
    if (s <= 61) return 31;
    return dag_node_26(s);
}
static __attribute__((noinline)) int dag_node_28(unsigned s) {
    if (s <= 60) return dag_node_25(s);
    return dag_node_27(s);
}
static __attribute__((noinline)) int dag_node_29(unsigned s) {
    if (s <= 58) return dag_node_24(s);
    return dag_node_28(s);
}
static __attribute__((noinline)) int dag_node_30(unsigned s) {
    if (s <= 64) return 34;
    return 35;
}
static __attribute__((noinline)) int dag_node_31(unsigned s) {
    if (s <= 63) return dag_node_29(s);
    return dag_node_30(s);
}
static __attribute__((noinline)) int dag_node_32(unsigned s) {
    if (s <= 48) return dag_node_15(s);
    return dag_node_31(s);
}
static __attribute__((noinline)) int dag_node_33(unsigned s) {
    if (s <= 35) return dag_node_3(s);
    return dag_node_32(s);
}
static __attribute__((noinline)) int dag_node_34(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_33(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_34(s);
}
