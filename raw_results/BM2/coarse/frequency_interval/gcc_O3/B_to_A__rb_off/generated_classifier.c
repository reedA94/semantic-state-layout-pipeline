#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 55

#define DEAD_STATE 0

#define NUM_CLASSES 14

#define NUM_TOKENS 39

static const char *CLASS_NAMES[14] = {
    "DEAD",
    "ACC::IDENT",
    "ACC::WHITESPACE",
    "ACC::DELIM",
    "ACC::COMMENT",
    "ACC::NUMBER",
    "ACC::OPERATOR",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::STRING_BODY",
    "NONACC::NUMBER_PREFIX",
    "ACC::STRING",
    "NONACC::STRING_ESCAPE",
    "ACC::KEYWORD",
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

static const int CLASS_ACCEPTING[14] = {
    0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0,
};

static const int CLASS_DEAD[14] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 11, 38, 21, 33, 18, 31, 19, 32, 34, 5, 6, 12, 8, 35, 3,
    7, 4, 25, 22, 20, 10, 9, 29, 30, 23, 24, 2, 0, 1, 28, 27,
    36, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    37, 26, 14, 15, 13, 17, 16, 26,
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
    if (s <= 10) return 3;
    if (s <= 11) return 4;
    if (s <= 13) return 5;
    if (s <= 32) return 6;
    if (s <= 45) return 7;
    if (s <= 46) return 8;
    if (s <= 47) return 9;
    if (s <= 48) return 10;
    if (s <= 49) return 11;
    if (s <= 54) return 12;
    return 13;
}
