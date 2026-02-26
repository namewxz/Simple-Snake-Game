#include "touch.hpp"
#include "point.hpp"
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <stdlib.h>
TouchEvent::TouchEvent()
{
    fd_ = ::open("/dev/input/event0", O_RDONLY);
    if (fd_ == -1)
        throw std::runtime_error("open touch error!");
}

TouchEvent::~TouchEvent()
{
    ::close(fd_);
}

Point TouchEvent::getPos()
{
    struct input_event event;
    Point pos;
    while (1)
    {
        int r = ::read(fd_, &event, sizeof(event));
        if (r == -1)
            throw std::runtime_error("touch read error!");
        if (event.type == EV_ABS && event.code == ABS_X)
        {
            pos.setX(event.value);
        }
        else if (event.type == EV_ABS && event.code == ABS_Y)
        {
            pos.setY(event.value);
        }
        else if (event.type == EV_KEY && event.value == 0)
        {
            break;
        }
    }
    return pos;
}

Direction TouchEvent::direction()
{
    struct input_event event;
    Point start;
    Point end;
    while (1)
    {
        int r = ::read(fd_, &event, sizeof(event));
        if (r == -1)
            throw std::runtime_error("touch read error!");
        if (event.type == EV_ABS && event.code == ABS_X)
        {
            if (start.x() == 0)
            {
                start.setX(event.value);
            }
            end.setX(event.value);
        }
        else if (event.type == EV_ABS && event.code == ABS_Y)
        {
            if (start.y() == 0)
            {
                start.setY(event.value);
            }
            end.setY(event.value);
        }
        else if (event.type == EV_KEY && event.value == 0)
        {
            break;
        }
    }

    return analyseDirection(start, end);
}

Direction TouchEvent::analyseDirection(Point start, Point end)
{
    // 如果水平方向的偏移>垂直方向的偏移，则为左或右
    if (abs(start.x() - end.x()) > abs(start.y() - end.y()))
    {
        if (start.x() > end.x())
            return Left;
        return Right;
    }
    else
    {
        if (start.y() > end.y())
            return Up;
        return Down;
    }
}
