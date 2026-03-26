#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 0

#define DEAD_STATE 35

#define NUM_CLASSES 2

#define NUM_TOKENS 31

static const char *CLASS_NAMES[2] = {
    "NONACC_OR_DEAD",
    "ACCEPTING",
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

static const int CLASS_ACCEPTING[2] = {
    0, 1,
};

static const int CLASS_DEAD[2] = {
    0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 6, 7, 3, 30, 29, 2, 20, 5, 18, 4, 22, 24,
    21, 28, 27, 0, 26, 19, 25, 1, 13, 9, 17, 8, 12, 10, 11, 16,
    15, 14,
};

static const int DEAD_OF_STATE[66] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

#define ACCEPT_THRESHOLD 36

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s >= ACCEPT_THRESHOLD) return 1;
    return 0;
}
