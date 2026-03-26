#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 30

#define START_STATE 13

#define DEAD_STATE 0

#define NUM_CLASSES 18

#define NUM_TOKENS 13

static const char *CLASS_NAMES[18] = {
    "DEAD",
    "NONACC::LITERAL_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::LITERAL::tok=FALSE::aux=LITERAL_PREFIX::prio1::rb1",
    "ACC::LITERAL::tok=NULL::aux=LITERAL_PREFIX::prio1::rb1",
    "ACC::LITERAL::tok=TRUE::aux=LITERAL_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=NUMBER::aux=NUMBER::prio0::rb1",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACE::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACE::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
};

static const char *TOKEN_NAMES[13] = {
    "COLON",
    "COMMA",
    "FALSE",
    "LBRACE",
    "LBRACK",
    "NO_TOKEN",
    "NULL",
    "NUMBER",
    "RBRACE",
    "RBRACK",
    "STRING",
    "TRUE",
    "WS",
};

static const int CLASS_ACCEPTING[18] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1,
};

static const int CLASS_DEAD[18] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

static const int TOKEN_OF_STATE[30] = {
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    2, 6, 11, 7, 7, 7, 0, 1, 3, 4, 8, 9, 10, 12,
};

static const int DEAD_OF_STATE[30] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 15) {
            if (s <= 12) {
                if (s <= 10) {
                    return 1;
                } else {
                    return 2;
                }
            } else {
                if (s <= 13) {
                    return 3;
                } else {
                    if (s <= 14) {
                        return 4;
                    } else {
                        return 5;
                    }
                }
            }
        } else {
            if (s <= 21) {
                if (s <= 18) {
                    if (s <= 16) {
                        return 6;
                    } else {
                        if (s <= 17) {
                            return 7;
                        } else {
                            return 8;
                        }
                    }
                } else {
                    return 9;
                }
            } else {
                if (s <= 27) {
                    if (s <= 24) {
                        if (s <= 22) {
                            return 10;
                        } else {
                            if (s <= 23) {
                                return 11;
                            } else {
                                return 12;
                            }
                        }
                    } else {
                        if (s <= 25) {
                            return 13;
                        } else {
                            if (s <= 26) {
                                return 14;
                            } else {
                                return 15;
                            }
                        }
                    }
                } else {
                    if (s <= 28) {
                        return 16;
                    } else {
                        return 17;
                    }
                }
            }
        }
    }
}

