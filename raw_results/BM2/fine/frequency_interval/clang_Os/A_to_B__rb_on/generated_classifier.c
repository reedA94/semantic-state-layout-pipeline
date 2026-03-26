#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 55

#define DEAD_STATE 0

#define NUM_CLASSES 44

#define NUM_TOKENS 39

static const char *CLASS_NAMES[44] = {
    "DEAD",
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0::rb1",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::COMMENT::tok=COMMENT_LINE::aux=COMMENT::prio0::rb0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0::rb0",
    "NONACC::STRING_BODY",
    "NONACC::KEYWORD_PREFIX",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0::rb1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "NONACC::NUMBER_PREFIX",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb1",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=BANG::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0::rb0",
    "NONACC::STRING_ESCAPE",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=LBRACK::aux=DELIM::prio0::rb0",
    "ACC::DELIM::tok=RBRACK::aux=DELIM::prio0::rb0",
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_INT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_WHILE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::OPERATOR::tok=AMP::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=ANDAND::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=ARROW::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=MINUSMINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=OROR::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PIPE::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=PLUSPLUS::aux=OP::prio0::rb0",
    "NONACC::START",
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
    0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
};

static const int CLASS_DEAD[44] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 11, 38, 6, 12, 34, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 3, 8, 26, 18, 21, 31, 33, 29, 37, 35, 22, 10,
    20, 7, 23, 9, 36, 4, 25, 26, 5, 19, 32, 13, 14, 15, 16, 17,
    0, 1, 2, 24, 27, 28, 30, 26,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 3) return 3;
    if (s <= 4) return 4;
    if (s <= 5) return 5;
    if (s <= 6) return 6;
    if (s <= 19) return 7;
    if (s <= 20) return 8;
    if (s <= 21) return 9;
    if (s <= 22) return 10;
    if (s <= 23) return 11;
    if (s <= 24) return 12;
    if (s <= 25) return 13;
    if (s <= 26) return 14;
    if (s <= 27) return 15;
    if (s <= 28) return 16;
    if (s <= 29) return 17;
    if (s <= 30) return 18;
    if (s <= 31) return 19;
    if (s <= 32) return 20;
    if (s <= 33) return 21;
    if (s <= 34) return 22;
    if (s <= 35) return 23;
    if (s <= 36) return 24;
    if (s <= 37) return 25;
    if (s <= 38) return 26;
    if (s <= 39) return 27;
    if (s <= 40) return 28;
    if (s <= 41) return 29;
    if (s <= 42) return 30;
    if (s <= 43) return 31;
    if (s <= 44) return 32;
    if (s <= 45) return 33;
    if (s <= 46) return 34;
    if (s <= 47) return 35;
    if (s <= 48) return 36;
    if (s <= 49) return 37;
    if (s <= 50) return 38;
    if (s <= 51) return 39;
    if (s <= 52) return 40;
    if (s <= 53) return 41;
    if (s <= 54) return 42;
    return 43;
}
