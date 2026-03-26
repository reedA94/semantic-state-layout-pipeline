#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 56

#define START_STATE 15

#define DEAD_STATE 0

#define NUM_CLASSES 44

#define NUM_TOKENS 39

static const char *CLASS_NAMES[44] = {
    "DEAD",
    "NONACC::KEYWORD_PREFIX",
    "NONACC::NUMBER_PREFIX",
    "NONACC::START",
    "NONACC::STRING_BODY",
    "NONACC::STRING_ESCAPE",
    "ACC::COMMENT::tok=COMMENT_LINE::aux=COMMENT::prio0",
    "ACC::DELIM::tok=COMMA::aux=DELIM::prio0",
    "ACC::DELIM::tok=LBRACE::aux=DELIM::prio0",
    "ACC::DELIM::tok=LBRACK::aux=DELIM::prio0",
    "ACC::DELIM::tok=LPAR::aux=DELIM::prio0",
    "ACC::DELIM::tok=RBRACE::aux=DELIM::prio0",
    "ACC::DELIM::tok=RBRACK::aux=DELIM::prio0",
    "ACC::DELIM::tok=RPAR::aux=DELIM::prio0",
    "ACC::DELIM::tok=SEMI::aux=DELIM::prio0",
    "ACC::IDENT::tok=IDENT::aux=WORD::prio0",
    "ACC::KEYWORD::tok=KW_FOR::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_IF::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_INT::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_RETURN::aux=KEYWORD_PREFIX::prio1",
    "ACC::KEYWORD::tok=KW_WHILE::aux=KEYWORD_PREFIX::prio1",
    "ACC::NUMBER::tok=FLOAT::aux=NUMBER::prio0",
    "ACC::NUMBER::tok=INT::aux=NUMBER::prio0",
    "ACC::OPERATOR::tok=AMP::aux=OP::prio0",
    "ACC::OPERATOR::tok=ANDAND::aux=OP::prio0",
    "ACC::OPERATOR::tok=ARROW::aux=OP::prio0",
    "ACC::OPERATOR::tok=ASSIGN::aux=OP::prio0",
    "ACC::OPERATOR::tok=BANG::aux=OP::prio0",
    "ACC::OPERATOR::tok=EQEQ::aux=OP::prio0",
    "ACC::OPERATOR::tok=GE::aux=OP::prio0",
    "ACC::OPERATOR::tok=GT::aux=OP::prio0",
    "ACC::OPERATOR::tok=LE::aux=OP::prio0",
    "ACC::OPERATOR::tok=LT::aux=OP::prio0",
    "ACC::OPERATOR::tok=MINUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=MINUSMINUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=NEQ::aux=OP::prio0",
    "ACC::OPERATOR::tok=OROR::aux=OP::prio0",
    "ACC::OPERATOR::tok=PIPE::aux=OP::prio0",
    "ACC::OPERATOR::tok=PLUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=PLUSPLUS::aux=OP::prio0",
    "ACC::OPERATOR::tok=SLASH::aux=OP::prio0",
    "ACC::OPERATOR::tok=STAR::aux=OP::prio0",
    "ACC::STRING::tok=STRING::aux=STRING::prio0",
    "ACC::WHITESPACE::tok=WS::aux=WS::prio0",
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
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static const int CLASS_DEAD[44] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[56] = {
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 6, 5, 18, 19, 21, 31, 32, 33, 34, 11, 13, 14, 15, 16,
    17, 8, 12, 0, 1, 2, 3, 4, 7, 9, 10, 20, 22, 23, 24, 25,
    27, 28, 29, 30, 35, 36, 37, 38,
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
        if (s <= 17) {
            if (s <= 14) {
                if (s <= 13) {
                    return 1;
                } else {
                    return 2;
                }
            } else {
                if (s <= 15) {
                    return 3;
                } else {
                    if (s <= 16) {
                        return 4;
                    } else {
                        return 5;
                    }
                }
            }
        } else {
            if (s <= 32) {
                if (s <= 26) {
                    if (s <= 18) {
                        return 6;
                    } else {
                        if (s <= 22) {
                            if (s <= 20) {
                                if (s <= 19) {
                                    return 7;
                                } else {
                                    return 8;
                                }
                            } else {
                                if (s <= 21) {
                                    return 9;
                                } else {
                                    return 10;
                                }
                            }
                        } else {
                            if (s <= 24) {
                                if (s <= 23) {
                                    return 11;
                                } else {
                                    return 12;
                                }
                            } else {
                                if (s <= 25) {
                                    return 13;
                                } else {
                                    return 14;
                                }
                            }
                        }
                    }
                } else {
                    if (s <= 27) {
                        return 15;
                    } else {
                        if (s <= 29) {
                            if (s <= 28) {
                                return 16;
                            } else {
                                return 17;
                            }
                        } else {
                            if (s <= 30) {
                                return 18;
                            } else {
                                if (s <= 31) {
                                    return 19;
                                } else {
                                    return 20;
                                }
                            }
                        }
                    }
                }
            } else {
                if (s <= 53) {
                    if (s <= 34) {
                        if (s <= 33) {
                            return 21;
                        } else {
                            return 22;
                        }
                    } else {
                        if (s <= 43) {
                            if (s <= 38) {
                                if (s <= 36) {
                                    if (s <= 35) {
                                        return 23;
                                    } else {
                                        return 24;
                                    }
                                } else {
                                    if (s <= 37) {
                                        return 25;
                                    } else {
                                        return 26;
                                    }
                                }
                            } else {
                                if (s <= 40) {
                                    if (s <= 39) {
                                        return 27;
                                    } else {
                                        return 28;
                                    }
                                } else {
                                    if (s <= 41) {
                                        return 29;
                                    } else {
                                        if (s <= 42) {
                                            return 30;
                                        } else {
                                            return 31;
                                        }
                                    }
                                }
                            }
                        } else {
                            if (s <= 48) {
                                if (s <= 45) {
                                    if (s <= 44) {
                                        return 32;
                                    } else {
                                        return 33;
                                    }
                                } else {
                                    if (s <= 46) {
                                        return 34;
                                    } else {
                                        if (s <= 47) {
                                            return 35;
                                        } else {
                                            return 36;
                                        }
                                    }
                                }
                            } else {
                                if (s <= 50) {
                                    if (s <= 49) {
                                        return 37;
                                    } else {
                                        return 38;
                                    }
                                } else {
                                    if (s <= 51) {
                                        return 39;
                                    } else {
                                        if (s <= 52) {
                                            return 40;
                                        } else {
                                            return 41;
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if (s <= 54) {
                        return 42;
                    } else {
                        return 43;
                    }
                }
            }
        }
    }
}

