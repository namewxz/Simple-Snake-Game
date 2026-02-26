#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "screen.hpp"
#include "color.hpp"

class Shape {
public:
    static void drawCircle(Screen& screen, int x, int y, int radius, const Color& color);
    static void drawTriangle(Screen& screen, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color);
    static void drawRect(Screen& screen, int x, int y, int width, int height, const Color& color);
};

#endif
