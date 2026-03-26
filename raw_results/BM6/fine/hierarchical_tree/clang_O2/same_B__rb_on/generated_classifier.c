#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 23

#define DEAD_STATE 0

#define NUM_CLASSES 33

#define NUM_TOKENS 28

static const char *CLASS_NAMES[33] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::tok=COMMENT::aux=COMMENT::prio0::rb0",
    "ACC::IDENT::tok=IDENT::aux=IDENT::prio0::rb1",
    "ACC::KEYWORD::tok=KW_CLASS::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_DEF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_ELSE::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IMPORT::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_IN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1::rb1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0::rb1",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0::rb1",
    "ACC::OPERATOR::tok=EQ::aux=OP::prio0::rb1",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0::rb0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0::rb0",
    "ACC::PUNCT::tok=COLON::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=COMMA::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=DOT::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=LPAR::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RBRACK::aux=PUNCT::prio0::rb0",
    "ACC::PUNCT::tok=RPAR::aux=PUNCT::prio0::rb0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0::rb0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0::rb0",
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
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1,
};

static const int CLASS_DEAD[33] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,
};

static const int TOKEN_OF_STATE[56] = {
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 2, 7, 9, 10,
    11, 12, 13, 14, 15, 16, 6, 8, 4, 5, 19, 21, 24, 25, 0, 1,
    3, 17, 18, 22, 23, 26, 26, 27,
};

static const int DEAD_OF_STATE[56] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 0) {
        return 0;
    } else {
        if (s <= 27) {
            if (s <= 22) {
                if (s <= 21) {
                    return 1;
                } else {
                    return 2;
                }
            } else {
                if (s <= 23) {
                    return 3;
                } else {
                    if (s <= 25) {
                        return 4;
                    } else {
                        return 5;
                    }
                }
            }
        } else {
            if (s <= 39) {
                if (s <= 29) {
                    if (s <= 28) {
                        return 6;
                    } else {
                        return 7;
                    }
                } else {
                    if (s <= 37) {
                        if (s <= 33) {
                            if (s <= 31) {
                                if (s <= 30) {
                                    return 8;
                                } else {
                                    return 9;
                                }
                            } else {
                                if (s <= 32) {
                                    return 10;
                                } else {
                                    return 11;
                                }
                            }
                        } else {
                            if (s <= 35) {
                                if (s <= 34) {
                                    return 12;
                                } else {
                                    return 13;
                                }
                            } else {
                                if (s <= 36) {
                                    return 14;
                                } else {
                                    return 15;
                                }
                            }
                        }
                    } else {
                        if (s <= 38) {
                            return 16;
                        } else {
                            return 17;
                        }
                    }
                }
            } else {
                if (s <= 52) {
                    if (s <= 45) {
                        if (s <= 42) {
                            if (s <= 40) {
                                return 18;
                            } else {
                                if (s <= 41) {
                                    return 19;
                                } else {
                                    return 20;
                                }
                            }
                        } else {
                            if (s <= 43) {
                                return 21;
                            } else {
                                if (s <= 44) {
                                    return 22;
                                } else {
                                    return 23;
                                }
                            }
                        }
                    } else {
                        if (s <= 48) {
                            if (s <= 46) {
                                return 24;
                            } else {
                                if (s <= 47) {
                                    return 25;
                                } else {
                                    return 26;
                                }
                            }
                        } else {
                            if (s <= 50) {
                                if (s <= 49) {
                                    return 27;
                                } else {
                                    return 28;
                                }
                            } else {
                                if (s <= 51) {
                                    return 29;
                                } else {
                                    return 30;
                                }
                            }
                        }
                    }
                } else {
                    if (s <= 54) {
                        return 31;
                    } else {
                        return 32;
                    }
                }
            }
        }
    }
}

