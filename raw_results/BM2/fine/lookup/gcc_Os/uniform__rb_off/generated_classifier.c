#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 14

#define DEAD_STATE 2

#define NUM_CLASSES 44

#define NUM_TOKENS 39

static const char *CLASS_NAMES[44] = {
    "NONACC::KEYWORD_PREFIX",
    "DEAD",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0",
    "NONACC::STRING_ESCAPE",
    "ACC::OPERATOR::tok=ARROW::aux=OP::prio0",
    "ACC::OPERATOR::tok=PLUSPLUS::aux=OP::prio0",
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0",
    "NONACC::START",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0",
    "ACC::DELIM::tok=LBRACK::aux=DELIM::prio0",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0",
    "NONACC::NUMBER_PREFIX",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0",
    "ACC::COMMENT::tok=COMMENT_LINE::aux=COMMENT::prio0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=OROR::aux=OP::prio0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0",
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0",
    "ACC::DELIM::tok=RBRACK::aux=DELIM::prio0",
    "NONACC::STRING_BODY",
    "ACC::KEYWORD::tok=KW_INT::aux=KEYWORD_PREFIX::prio1",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::OPERATOR::tok=BANG::aux=OP::prio0",
    "ACC::OPERATOR::tok=MINUSMINUS::aux=OP::prio0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=ANDAND::aux=OP::prio0",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0",
    "ACC::OPERATOR::tok=PIPE::aux=OP::prio0",
    "ACC::KEYWORD::tok=KW_WHILE::aux=KEYWORD_PREFIX::prio1",
    "ACC::OPERATOR::tok=AMP::aux=OP::prio0",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1",
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
    0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[44] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 26, 26, 38, 26, 26, 34, 26, 2, 30, 11, 5, 26, 7, 26, 14,
    25, 26, 26, 26, 19, 21, 26, 3, 26, 10, 22, 6, 23, 27, 36, 8,
    13, 26, 35, 32, 26, 26, 15, 9, 37, 26, 4, 26, 24, 33, 29, 1,
    18, 31, 20, 12, 28, 17, 0, 16,
};

static const int DEAD_OF_STATE[56] = {
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_OF_STATE[56] = {
    0, 0, 1, 2, 0, 0, 3, 4, 5, 6, 7, 8, 0, 9, 10, 11,
    12, 0, 0, 0, 13, 14, 0, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 0, 25, 26, 0, 27, 28, 29, 30, 0, 31, 0, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    return CLASS_OF_STATE[s];
}
