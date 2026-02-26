#ifndef TOUCHEVENT_HPP
#define TOUCHEVENT_HPP

#include <string>
#include "point.hpp"
enum Direction {Error, Up, Down, Left, Right, None};
class TouchEvent
{
public:
    TouchEvent();
    ~TouchEvent();
    Direction direction();
    Point getPos();
    struct TouchData {
        Point pos;
        Direction dir;
    };
    TouchData getTouchEvent();
private:
    Direction analyseDirection(Point start, Point end);
    int fd_;
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 480;
    const int TOUCH_WIDTH = 1024;
    const int TOUCH_HEIGHT = 600;
};

#endif