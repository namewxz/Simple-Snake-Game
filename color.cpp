#include "color.hpp"


Color::Color(int r, int g, int b):_red(r), _green(g), _blue(b) {}

void Color::setColor(int r, int g, int b)
{
    _red = r;
    _green = g;
    _blue = b;
}

int Color::red() const
{
    return _red;
}

int Color::value() const
{
    int v = _red << 16 | _green << 8 | _blue;
    return v;
}