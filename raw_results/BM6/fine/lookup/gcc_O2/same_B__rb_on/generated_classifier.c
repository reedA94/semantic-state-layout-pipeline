#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 14

#define DEAD_STATE 2

#define NUM_CLASSES 33

#define NUM_TOKENS 28

static const char *CLASS_NAMES[33] = {
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1::rb1",
    "NONACC::KEYWORD_PREFIX",
    "DEAD",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1::rb1",
    "NONACC::STRING_ESCAPE",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::KEYWORD::tok=KW_ELSE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0::rb0",
    "NONACC::START",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "ACC::KEYWORD::tok=KW_CLASS::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "NONACC::NUMBER_PREFIX",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::KEYWORD::tok=KW_DEF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "NONACC::STRING_BODY",
    "ACC::KEYWORD::tok=KW_IMPORT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb1",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::KEYWORD::tok=KW_IN::aux=KEYWORD_PREFIX::prio1::rb1",
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
    1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static const int CLASS_DEAD[33] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[56] = {
    12, 20, 20, 27, 16, 20, 13, 20, 23, 1, 7, 20, 11, 2, 20, 20,
    5, 20, 20, 20, 9, 20, 20, 26, 20, 19, 0, 20, 3, 20, 10, 6,
    20, 20, 20, 20, 14, 20, 20, 25, 26, 20, 4, 20, 18, 20, 24, 22,
    20, 20, 21, 8, 20, 20, 17, 15,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_OF_STATE[56] = {
    0, 1, 2, 3, 4, 1, 5, 6, 7, 8, 9, 1, 10, 11, 12, 1,
    13, 1, 1, 1, 14, 1, 1, 15, 16, 17, 18, 6, 19, 1, 20, 21,
    1, 1, 22, 1, 23, 22, 1, 24, 15, 1, 25, 1, 26, 1, 27, 28,
    1, 1, 29, 30, 1, 1, 31, 32,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
