#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 14

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 14

#define NUM_TOKENS 11

static const char *CLASS_NAMES[14] = {
    "DEAD",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=EQ::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::VALUE::tok=BARE_VALUE::aux=VALUE::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
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

static const int CLASS_ACCEPTING[14] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[14] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[14] = {
    6, 6, 6, 6, 1, 4, 7, 2, 3, 5, 8, 9, 0, 10,
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
                        if (s <= 8) {
                            if (s <= 7) {
                                return 7;
                            } else {
                                return 8;
                            }
                        } else {
                            if (s <= 9) {
                                return 9;
                            } else {
                                return 10;
                            }
                        }
                    } else {
                        return 11;
                    }
                } else {
                    if (s <= 12) {
                        return 12;
                    } else {
                        return 13;
                    }
                }
            }
        }
    }
}

