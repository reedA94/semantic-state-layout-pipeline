#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 20

#define DEAD_STATE 47

#define NUM_CLASSES 36

#define NUM_TOKENS 31

static const char *CLASS_NAMES[36] = {
    "NONACC::KEYWORD_PREFIX",
    "NONACC::STRING_ESCAPE",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::KEYWORD::tok=KW_AND::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_FROM::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_INTO::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_UPDATE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0::rb1",
    "NONACC::NUMBER_PREFIX",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0::rb1",
    "ACC::KEYWORD::tok=KW_SELECT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0::rb0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "NONACC::START",
    "ACC::KEYWORD::tok=KW_VALUES::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::KEYWORD::tok=KW_OR::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::PUNCT::tok=SEMI::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb0",
    "ACC::KEYWORD::tok=KW_WHERE::aux=KEYWORD_PREFIX::prio1::rb1",
    "NONACC::STRING_BODY",
    "DEAD",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::KEYWORD::tok=KW_SET::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
    "ACC::KEYWORD::tok=KW_INSERT::aux=KEYWORD_PREFIX::prio1::rb1",
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
    0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static const int CLASS_DEAD[36] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    23, 23, 23, 23, 23, 23, 23, 1, 8, 23, 9, 11, 15, 5, 23, 20,
    23, 13, 18, 7, 23, 16, 6, 23, 23, 22, 24, 28, 12, 23, 23, 26,
    23, 23, 23, 23, 23, 19, 3, 23, 23, 2, 17, 23, 23, 23, 23, 23,
    27, 29, 23, 4, 23, 25, 23, 0, 14, 23, 23, 23, 23, 23, 21, 30,
    23, 10,
};

static const int DEAD_OF_STATE[66] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const int CLASS_OF_STATE[66] = {
    0, 0, 0, 1, 0, 0, 0, 2, 3, 0, 4, 5, 6, 7, 8, 9,
    0, 10, 11, 12, 13, 14, 15, 0, 0, 16, 17, 18, 19, 0, 0, 20,
    0, 0, 0, 0, 0, 21, 22, 0, 0, 23, 24, 0, 0, 0, 25, 26,
    27, 28, 0, 29, 0, 30, 0, 31, 32, 0, 0, 0, 0, 0, 33, 34,
    0, 35,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
