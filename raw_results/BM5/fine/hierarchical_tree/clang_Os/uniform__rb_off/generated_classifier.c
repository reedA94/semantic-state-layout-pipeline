#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 66

#define START_STATE 33

#define DEAD_STATE 0

#define NUM_CLASSES 36

#define NUM_TOKENS 31

static const char *CLASS_NAMES[36] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0",
    "ACC::KEYWORD::tok=KW_AND::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_FROM::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_INSERT::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_INTO::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_OR::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_SELECT::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_SET::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_UPDATE::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_VALUES::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_WHERE::aux=KEYWORD_PREFIX::prio1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0",
    "ACC::PUNCT::tok=SEMI::aux=PUNCT::prio0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
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
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1,
};

static const int CLASS_DEAD[36] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[66] = {
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    23, 23, 23, 23, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 3,
    7, 2, 4, 5, 18, 20, 21, 22, 24, 27, 28, 0, 1, 19, 25, 26,
    29, 30,
};

static const int DEAD_OF_STATE[66] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 35) {
            if (s <= 32) {
                if (s <= 31) {
                    return 1;
                } else {
                    return 2;
                }
            } else {
                if (s <= 33) {
                    return 3;
                } else {
                    if (s <= 34) {
                        return 4;
                    } else {
                        return 5;
                    }
                }
            }
        } else {
            if (s <= 48) {
                if (s <= 36) {
                    return 6;
                } else {
                    if (s <= 46) {
                        if (s <= 41) {
                            if (s <= 38) {
                                if (s <= 37) {
                                    return 7;
                                } else {
                                    return 8;
                                }
                            } else {
                                if (s <= 39) {
                                    return 9;
                                } else {
                                    if (s <= 40) {
                                        return 10;
                                    } else {
                                        return 11;
                                    }
                                }
                            }
                        } else {
                            if (s <= 43) {
                                if (s <= 42) {
                                    return 12;
                                } else {
                                    return 13;
                                }
                            } else {
                                if (s <= 44) {
                                    return 14;
                                } else {
                                    if (s <= 45) {
                                        return 15;
                                    } else {
                                        return 16;
                                    }
                                }
                            }
                        }
                    } else {
                        if (s <= 47) {
                            return 17;
                        } else {
                            return 18;
                        }
                    }
                }
            } else {
                if (s <= 63) {
                    if (s <= 58) {
                        if (s <= 53) {
                            if (s <= 50) {
                                if (s <= 49) {
                                    return 19;
                                } else {
                                    return 20;
                                }
                            } else {
                                if (s <= 51) {
                                    return 21;
                                } else {
                                    if (s <= 52) {
                                        return 22;
                                    } else {
                                        return 23;
                                    }
                                }
                            }
                        } else {
                            if (s <= 55) {
                                if (s <= 54) {
                                    return 24;
                                } else {
                                    return 25;
                                }
                            } else {
                                if (s <= 56) {
                                    return 26;
                                } else {
                                    if (s <= 57) {
                                        return 27;
                                    } else {
                                        return 28;
                                    }
                                }
                            }
                        }
                    } else {
                        if (s <= 60) {
                            if (s <= 59) {
                                return 29;
                            } else {
                                return 30;
                            }
                        } else {
                            if (s <= 61) {
                                return 31;
                            } else {
                                if (s <= 62) {
                                    return 32;
                                } else {
                                    return 33;
                                }
                            }
                        }
                    }
                } else {
                    if (s <= 64) {
                        return 34;
                    } else {
                        return 35;
                    }
                }
            }
        }
    }
}

