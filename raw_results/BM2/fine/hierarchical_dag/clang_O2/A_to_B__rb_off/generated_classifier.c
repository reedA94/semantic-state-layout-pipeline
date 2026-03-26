#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 15

#define DEAD_STATE 0

#define NUM_CLASSES 44

#define NUM_TOKENS 39

static const char *CLASS_NAMES[44] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::tok=COMMENT_LINE::aux=COMMENT::prio0",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0",
    "ACC::DELIM::tok=LBRACK::aux=DELIM::prio0",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0",
    "ACC::DELIM::tok=RBRACK::aux=DELIM::prio0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0",
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0",
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_INT::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_WHILE::aux=KEYWORD_PREFIX::prio1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0",
    "ACC::OPERATOR::tok=AMP::aux=OP::prio0",
    "ACC::OPERATOR::tok=ANDAND::aux=OP::prio0",
    "ACC::OPERATOR::tok=ARROW::aux=OP::prio0",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0",
    "ACC::OPERATOR::tok=BANG::aux=OP::prio0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=MINUSMINUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0",
    "ACC::OPERATOR::tok=OROR::aux=OP::prio0",
    "ACC::OPERATOR::tok=PIPE::aux=OP::prio0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=PLUSPLUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
};

static const char *TOKEN_NAMES[39] = {
    "AMP",
    "ANDAND",
    "ARROW",
    "ASSIGN",
    "BANG",
    "COMMA",
    "COMMENT_LINE",
    "EQEQ",
    "FLOAT",
    "GE",
    "GT",
    "IDENT",
    "INT",
    "KW_FOR",
    "KW_IF",
    "KW_INT",
    "KW_RETURN",
    "KW_WHILE",
    "LBRACE",
    "LBRACK",
    "LE",
    "LPAR",
    "LT",
    "MINUS",
    "MINUSMINUS",
    "NEQ",
    "NO_TOKEN",
    "OROR",
    "PIPE",
    "PLUS",
    "PLUSPLUS",
    "RBRACE",
    "RBRACK",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[44] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[44] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 6, 5, 18, 19, 21, 31, 32, 33, 34, 11, 13, 14, 15, 16,
    17, 8, 12, 0, 1, 2, 3, 4, 7, 9, 10, 20, 22, 23, 24, 25,
    27, 28, 29, 30, 35, 36, 37, 38,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static __attribute__((noinline)) int dag_node_0(unsigned s) {
    if (s <= 13) return 1;
    return 2;
}
static __attribute__((noinline)) int dag_node_1(unsigned s) {
    if (s <= 16) return 4;
    return 5;
}
static __attribute__((noinline)) int dag_node_2(unsigned s) {
    if (s <= 15) return 3;
    return dag_node_1(s);
}
static __attribute__((noinline)) int dag_node_3(unsigned s) {
    if (s <= 14) return dag_node_0(s);
    return dag_node_2(s);
}
static __attribute__((noinline)) int dag_node_4(unsigned s) {
    if (s <= 19) return 7;
    return 8;
}
static __attribute__((noinline)) int dag_node_5(unsigned s) {
    if (s <= 21) return 9;
    return 10;
}
static __attribute__((noinline)) int dag_node_6(unsigned s) {
    if (s <= 20) return dag_node_4(s);
    return dag_node_5(s);
}
static __attribute__((noinline)) int dag_node_7(unsigned s) {
    if (s <= 23) return 11;
    return 12;
}
static __attribute__((noinline)) int dag_node_8(unsigned s) {
    if (s <= 25) return 13;
    return 14;
}
static __attribute__((noinline)) int dag_node_9(unsigned s) {
    if (s <= 24) return dag_node_7(s);
    return dag_node_8(s);
}
static __attribute__((noinline)) int dag_node_10(unsigned s) {
    if (s <= 22) return dag_node_6(s);
    return dag_node_9(s);
}
static __attribute__((noinline)) int dag_node_11(unsigned s) {
    if (s <= 18) return 6;
    return dag_node_10(s);
}
static __attribute__((noinline)) int dag_node_12(unsigned s) {
    if (s <= 28) return 16;
    return 17;
}
static __attribute__((noinline)) int dag_node_13(unsigned s) {
    if (s <= 31) return 19;
    return 20;
}
static __attribute__((noinline)) int dag_node_14(unsigned s) {
    if (s <= 30) return 18;
    return dag_node_13(s);
}
static __attribute__((noinline)) int dag_node_15(unsigned s) {
    if (s <= 29) return dag_node_12(s);
    return dag_node_14(s);
}
static __attribute__((noinline)) int dag_node_16(unsigned s) {
    if (s <= 27) return 15;
    return dag_node_15(s);
}
static __attribute__((noinline)) int dag_node_17(unsigned s) {
    if (s <= 26) return dag_node_11(s);
    return dag_node_16(s);
}
static __attribute__((noinline)) int dag_node_18(unsigned s) {
    if (s <= 33) return 21;
    return 22;
}
static __attribute__((noinline)) int dag_node_19(unsigned s) {
    if (s <= 35) return 23;
    return 24;
}
static __attribute__((noinline)) int dag_node_20(unsigned s) {
    if (s <= 37) return 25;
    return 26;
}
static __attribute__((noinline)) int dag_node_21(unsigned s) {
    if (s <= 36) return dag_node_19(s);
    return dag_node_20(s);
}
static __attribute__((noinline)) int dag_node_22(unsigned s) {
    if (s <= 39) return 27;
    return 28;
}
static __attribute__((noinline)) int dag_node_23(unsigned s) {
    if (s <= 42) return 30;
    return 31;
}
static __attribute__((noinline)) int dag_node_24(unsigned s) {
    if (s <= 41) return 29;
    return dag_node_23(s);
}
static __attribute__((noinline)) int dag_node_25(unsigned s) {
    if (s <= 40) return dag_node_22(s);
    return dag_node_24(s);
}
static __attribute__((noinline)) int dag_node_26(unsigned s) {
    if (s <= 38) return dag_node_21(s);
    return dag_node_25(s);
}
static __attribute__((noinline)) int dag_node_27(unsigned s) {
    if (s <= 44) return 32;
    return 33;
}
static __attribute__((noinline)) int dag_node_28(unsigned s) {
    if (s <= 47) return 35;
    return 36;
}
static __attribute__((noinline)) int dag_node_29(unsigned s) {
    if (s <= 46) return 34;
    return dag_node_28(s);
}
static __attribute__((noinline)) int dag_node_30(unsigned s) {
    if (s <= 45) return dag_node_27(s);
    return dag_node_29(s);
}
static __attribute__((noinline)) int dag_node_31(unsigned s) {
    if (s <= 49) return 37;
    return 38;
}
static __attribute__((noinline)) int dag_node_32(unsigned s) {
    if (s <= 52) return 40;
    return 41;
}
static __attribute__((noinline)) int dag_node_33(unsigned s) {
    if (s <= 51) return 39;
    return dag_node_32(s);
}
static __attribute__((noinline)) int dag_node_34(unsigned s) {
    if (s <= 50) return dag_node_31(s);
    return dag_node_33(s);
}
static __attribute__((noinline)) int dag_node_35(unsigned s) {
    if (s <= 48) return dag_node_30(s);
    return dag_node_34(s);
}
static __attribute__((noinline)) int dag_node_36(unsigned s) {
    if (s <= 43) return dag_node_26(s);
    return dag_node_35(s);
}
static __attribute__((noinline)) int dag_node_37(unsigned s) {
    if (s <= 34) return dag_node_18(s);
    return dag_node_36(s);
}
static __attribute__((noinline)) int dag_node_38(unsigned s) {
    if (s <= 54) return 42;
    return 43;
}
static __attribute__((noinline)) int dag_node_39(unsigned s) {
    if (s <= 53) return dag_node_37(s);
    return dag_node_38(s);
}
static __attribute__((noinline)) int dag_node_40(unsigned s) {
    if (s <= 32) return dag_node_17(s);
    return dag_node_39(s);
}
static __attribute__((noinline)) int dag_node_41(unsigned s) {
    if (s <= 17) return dag_node_3(s);
    return dag_node_40(s);
}
static __attribute__((noinline)) int dag_node_42(unsigned s) {
    if (s <= 0) return 0;
    return dag_node_41(s);
}
__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return dag_node_42(s);
}
