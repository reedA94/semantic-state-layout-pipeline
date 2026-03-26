#include <stddef.h>

#include <stdint.h>

#define NUM_STATES 263

#define START_STATE 262

#define DEAD_STATE 64

#define NUM_CLASSES 15

#define NUM_TOKENS 9

static const char *CLASS_NAMES[15] = {
    "ACC::SFAM_0::tok=TOK_0::aux=AUX_0::prio0::rb0",
    "ACC::SFAM_0::tok=TOK_3::aux=AUX_0::prio1::rb1",
    "ACC::SFAM_0::tok=TOK_6::aux=AUX_0::prio0::rb1",
    "ACC::SFAM_1::tok=TOK_1::aux=AUX_1::prio1::rb0",
    "ACC::SFAM_1::tok=TOK_4::aux=AUX_1::prio0::rb0",
    "ACC::SFAM_1::tok=TOK_7::aux=AUX_1::prio1::rb1",
    "ACC::SFAM_2::tok=TOK_2::aux=AUX_2::prio0::rb1",
    "ACC::SFAM_2::tok=TOK_5::aux=AUX_2::prio1::rb0",
    "DEAD",
    "NONACC::PREFIX_0",
    "NONACC::PREFIX_1",
    "NONACC::PREFIX_2",
    "NONACC::PREFIX_3",
    "NONACC::PREFIX_4",
    "NONACC::START",
};

static const char *TOKEN_NAMES[9] = {
    "NO_TOKEN",
    "TOK_0",
    "TOK_1",
    "TOK_2",
    "TOK_3",
    "TOK_4",
    "TOK_5",
    "TOK_6",
    "TOK_7",
};

static const int CLASS_ACCEPTING[15] = {
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
};

static const int CLASS_DEAD[15] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
};

static const int TOKEN_OF_STATE[263] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 8, 8, 8, 8, 8, 8, 8,
    8, 3, 3, 3, 3, 3, 3, 3, 6, 6, 6, 6, 6, 6, 6, 6,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static const int DEAD_OF_STATE[263] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

__attribute__((noinline)) static int classify_semantic(unsigned s) {
    if (s <= 8) return 0;
    if (s <= 17) return 1;
    if (s <= 24) return 2;
    if (s <= 31) return 3;
    if (s <= 40) return 4;
    if (s <= 48) return 5;
    if (s <= 55) return 6;
    if (s <= 63) return 7;
    if (s <= 64) return 8;
    if (s <= 99) return 9;
    if (s <= 149) return 10;
    if (s <= 185) return 11;
    if (s <= 228) return 12;
    if (s <= 261) return 13;
    return 14;
}
