#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 17

#define START_STATE 1

#define DEAD_STATE 0

#define NUM_CLASSES 17

#define NUM_TOKENS 16

static const char *CLASS_NAMES[17] = {
    "DEAD",
    "NONACC::START",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0",
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
};

static const char *TOKEN_NAMES[16] = {
    "ASSIGN",
    "COMMA",
    "EQEQ",
    "IDENT",
    "INT",
    "LBRACE",
    "LPAR",
    "MINUS",
    "NO_TOKEN",
    "PLUS",
    "RBRACE",
    "RPAR",
    "SEMI",
    "SLASH",
    "STAR",
    "WS",
};

static const int CLASS_ACCEPTING[17] = {
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static const int CLASS_DEAD[17] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[17] = {
    8, 8, 1, 5, 6, 10, 11, 12, 3, 4, 0, 2, 7, 9, 13, 14,
    15,
};

static const int DEAD_OF_STATE[17] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 1) {
            return 1;
        } else {
            if (s <= 8) {
                if (s <= 7) {
                    if (s <= 4) {
                        if (s <= 2) {
                            return 2;
                        } else {
                            if (s <= 3) {
                                return 3;
                            } else {
                                return 4;
                            }
                        }
                    } else {
                        if (s <= 5) {
                            return 5;
                        } else {
                            if (s <= 6) {
                                return 6;
                            } else {
                                return 7;
                            }
                        }
                    }
                } else {
                    return 8;
                }
            } else {
                if (s <= 9) {
                    return 9;
                } else {
                    if (s <= 15) {
                        if (s <= 12) {
                            if (s <= 10) {
                                return 10;
                            } else {
                                if (s <= 11) {
                                    return 11;
                                } else {
                                    return 12;
                                }
                            }
                        } else {
                            if (s <= 13) {
                                return 13;
                            } else {
                                if (s <= 14) {
                                    return 14;
                                } else {
                                    return 15;
                                }
                            }
                        }
                    } else {
                        return 16;
                    }
                }
            }
        }
    }
}

