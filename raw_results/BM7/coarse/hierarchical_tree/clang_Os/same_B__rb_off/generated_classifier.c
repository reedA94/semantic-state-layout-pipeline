#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 11

#define NUM_TOKENS 11

static const char *CLASS_NAMES[11] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT",
    "ACC::IDENT",
    "ACC::NUMBER",
    "ACC::PUNCT",
    "ACC::STRING",
    "ACC::VALUE",
    "ACC::WHITESPACE",
};

static const char *TOKEN_NAMES[11] = {
    "BARE_VALUE",
    "COMMENT",
    "DOT",
    "EQ",
    "IDENT",
    "LBRACK",
    "NO_TOKEN",
    "NUMBER",
    "RBRACK",
    "STRING",
    "WS",
};

static const int CLASS_ACCEPTING[11] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[11] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    6, 6, 6, 6, 1, 4, 7, 5, 8, 3, 2, 9, 0, 10,
};

static const int DEAD_OF_STATE[14] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 3) {
            if (s <= 1) {
                return 1;
            } else {
                if (s <= 2) {
                    return 2;
                } else {
                    return 3;
                }
            }
        } else {
            if (s <= 6) {
                if (s <= 4) {
                    return 4;
                } else {
                    if (s <= 5) {
                        return 5;
                    } else {
                        return 6;
                    }
                }
            } else {
                if (s <= 11) {
                    if (s <= 10) {
                        return 7;
                    } else {
                        return 8;
                    }
                } else {
                    if (s <= 12) {
                        return 9;
                    } else {
                        return 10;
                    }
                }
            }
        }
    }
}

