#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 23

#define DEAD_STATE 0

#define NUM_CLASSES 33

#define NUM_TOKENS 28

static const char *CLASS_NAMES[33] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0::rb0",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::KEYWORD::tok=KW_CLASS::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_DEF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_ELSE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IMPORT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
};

static const char *TOKEN_NAMES[28] = {
    "COLON",
    "COMMA",
    "COMMENT",
    "DOT",
    "EQ",
    "EQEQ",
    "FLOAT",
    "IDENT",
    "INT",
    "KW_CLASS",
    "KW_DEF",
    "KW_ELSE",
    "KW_FOR",
    "KW_IF",
    "KW_IMPORT",
    "KW_IN",
    "KW_RETURN",
    "LBRACK",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACK",
    "RPAR",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[33] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static const int CLASS_DEAD[33] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[56] = {
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 2, 7, 9, 10,
    11, 12, 13, 14, 15, 16, 6, 8, 4, 5, 19, 21, 24, 25, 0, 1,
    3, 17, 18, 22, 23, 26, 26, 27,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 21) return 1;
    return 2;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 25) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 23) return 3;
    return dag_node_1(s);
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 22) return dag_node_0(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 28) return 6;
    return 7;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 30) return 8;
    return 9;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 32) return 10;
    return 11;
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 31) return dag_node_5(s);
    return dag_node_6(s);
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 34) return 12;
    return 13;
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 36) return 14;
    return 15;
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 35) return dag_node_8(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 33) return dag_node_7(s);
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 38) return 16;
    return 17;
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 37) return dag_node_11(s);
    return dag_node_12(s);
}
static __attribute__((noinline)) int dag_node_14(unsigned s) {
    if (s <= 29) return dag_node_4(s);
    return dag_node_13(s);
}
static __attribute__((noinline)) int dag_node_15(unsigned s) {
    if (s <= 41) return 19;
    return 20;
}
static __attribute__((noinline)) int dag_node_16(unsigned s) {
    if (s <= 40) return 18;
    return dag_node_15(s);
}
static __attribute__((noinline)) int dag_node_17(unsigned s) {
    if (s <= 44) return 22;
    return 23;
}
static __attribute__((noinline)) int dag_node_18(unsigned s) {
    if (s <= 43) return 21;
    return dag_node_17(s);
}
static __attribute__((noinline)) int dag_node_19(unsigned s) {
    if (s <= 42) return dag_node_16(s);
    return dag_node_18(s);
}
static __attribute__((noinline)) int dag_node_20(unsigned s) {
    if (s <= 47) return 25;
    return 26;
}
static __attribute__((noinline)) int dag_node_21(unsigned s) {
    if (s <= 46) return 24;
    return dag_node_20(s);
}
static __attribute__((noinline)) int dag_node_22(unsigned s) {
    if (s <= 49) return 27;
    return 28;
}
static __attribute__((noinline)) int dag_node_23(unsigned s) {
    if (s <= 51) return 29;
    return 30;
}
static __attribute__((noinline)) int dag_node_24(unsigned s) {
    if (s <= 50) return dag_node_22(s);
    return dag_node_23(s);
}
static __attribute__((noinline)) int dag_node_25(unsigned s) {
    if (s <= 48) return dag_node_21(s);
    return dag_node_24(s);
}
static __attribute__((noinline)) int dag_node_26(unsigned s) {
    if (s <= 45) return dag_node_19(s);
    return dag_node_25(s);
}
static __attribute__((noinline)) int dag_node_27(unsigned s) {
    if (s <= 54) return 31;
    return 32;
}
static __attribute__((noinline)) int dag_node_28(unsigned s) {
    if (s <= 52) return dag_node_26(s);
    return dag_node_27(s);
}
static __attribute__((noinline)) int dag_node_29(unsigned s) {
    if (s <= 39) return dag_node_14(s);
    return dag_node_28(s);
}
static __attribute__((noinline)) int dag_node_30(unsigned s) {
    if (s <= 27) return dag_node_3(s);
    return dag_node_29(s);
}
static __attribute__((noinline)) int dag_node_31(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_30(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_31(s);
}
