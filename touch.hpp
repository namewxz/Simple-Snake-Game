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
private:
    Direction analyseDirection(Point start, Point end);
    int fd_;
};


#endif