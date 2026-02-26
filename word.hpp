#ifndef WORD_DISPLAY_HPP
#define WORD_DISPLAY_HPP

#include <cstdint>
#include "screen.hpp"

class WordDisplay {
public:
    // Font data (made public for testing)
    static const uint8_t NUM_FONT[10][24*35/8];
    static const uint8_t NAME_FU_FONT[24*20/8];
    static const uint8_t NAME_TING_FONT[24*20/8];
    static const uint8_t NUM_1_FONT[16*19/8];

    // Display methods
    static void displayWord(Screen& screen, int x0, int y0, int w, int h, const uint8_t wordArr[], uint32_t color);
    static void displayNumber(Screen& screen, int x0, int y0, int w, int h, int num, uint32_t color);
    static void splitAndDisplayNumber(Screen& screen, int num, int high);
};

#endif // WORD_DISPLAY_HPP
