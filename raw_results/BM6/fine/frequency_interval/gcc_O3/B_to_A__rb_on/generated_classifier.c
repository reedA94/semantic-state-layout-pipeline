#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 53

#define DEAD_STATE 1

#define NUM_CLASSES 33

#define NUM_TOKENS 28

static const char *CLASS_NAMES[33] = {
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "DEAD",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0::rb0",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::STRING_BODY",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0::rb0",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb1",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::KEYWORD::tok=KW_CLASS::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_DEF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_ELSE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IMPORT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_ESCAPE",
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
    1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0,
};

static const int CLASS_DEAD[33] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[56] = {
    7, 20, 27, 2, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 0, 4, 8, 18, 23,
    5, 26, 26, 9, 10, 11, 12, 13, 14, 15, 16, 6, 19, 21, 24, 25,
    1, 3, 17, 22, 20, 20, 20, 20,
};

static const int DEAD_OF_STATE[56] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) return 0;
    if (s <= 1) return 1;
    if (s <= 2) return 2;
    if (s <= 3) return 3;
    if (s <= 24) return 4;
    if (s <= 26) return 5;
    if (s <= 27) return 6;
    if (s <= 28) return 7;
    if (s <= 29) return 8;
    if (s <= 30) return 9;
    if (s <= 31) return 10;
    if (s <= 32) return 11;
    if (s <= 34) return 12;
    if (s <= 35) return 13;
    if (s <= 36) return 14;
    if (s <= 37) return 15;
    if (s <= 38) return 16;
    if (s <= 39) return 17;
    if (s <= 40) return 18;
    if (s <= 41) return 19;
    if (s <= 42) return 20;
    if (s <= 43) return 21;
    if (s <= 44) return 22;
    if (s <= 45) return 23;
    if (s <= 46) return 24;
    if (s <= 47) return 25;
    if (s <= 48) return 26;
    if (s <= 49) return 27;
    if (s <= 50) return 28;
    if (s <= 51) return 29;
    if (s <= 52) return 30;
    if (s <= 53) return 31;
    return 32;
}
