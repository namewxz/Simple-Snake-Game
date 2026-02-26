#include "touch.hpp"
#include "point.hpp"
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <stdlib.h>
#include <iostream>
using namespace std;

TouchEvent::TouchData TouchEvent::getTouchEvent() {
    struct input_event event;
    Point start;
    Point end;
    Point currentPos;
    bool gotPos = false;
    bool gotDirection = false;
    TouchData data;

    while (1) {
        int r = ::read(fd_, &event, sizeof(event));
        if (r == -1)
            throw std::runtime_error("touch read error!");

        if (event.type == EV_ABS && event.code == ABS_X) {
            currentPos.setX(event.value);
            if (start.x() == 0) {
                start.setX(event.value);
            }
            end.setX(event.value);
        }
        else if (event.type == EV_ABS && event.code == ABS_Y) {
            currentPos.setY(event.value);
            if (start.y() == 0) {
                start.setY(event.value);
            }
            end.setY(event.value);
        }
        else if (event.type == EV_KEY && event.value == 0) {
            // 转换坐标到屏幕分辨率
            int screenX = static_cast<int>(currentPos.x() * static_cast<float>(SCREEN_WIDTH) / TOUCH_WIDTH);
            int screenY = static_cast<int>(currentPos.y() * static_cast<float>(SCREEN_HEIGHT) / TOUCH_HEIGHT);
            currentPos.setX(screenX);
            currentPos.setY(screenY);
            
            // 转换起点和终点坐标用于方向判断
            int startX = static_cast<int>(start.x() * static_cast<float>(SCREEN_WIDTH) / TOUCH_WIDTH);
            int startY = static_cast<int>(start.y() * static_cast<float>(SCREEN_HEIGHT) / TOUCH_HEIGHT);
            int endX = static_cast<int>(end.x() * static_cast<float>(SCREEN_WIDTH) / TOUCH_WIDTH);
            int endY = static_cast<int>(end.y() * static_cast<float>(SCREEN_HEIGHT) / TOUCH_HEIGHT);
            start.setX(startX);
            start.setY(startY);
            end.setX(endX);
            end.setY(endY);

            data.pos = currentPos;
            data.dir = analyseDirection(start, end);
            // cout << "touch pos: " << data.pos.x() << " " << data.pos.y() << endl;
            // if (data.dir != None) {
            //     cout << "swipe direction: ";
            //     switch(data.dir) {
            //         case Left: cout << "Left"; break;
            //         case Right: cout << "Right"; break;
            //         case Up: cout << "Up"; break;
            //         case Down: cout << "Down"; break;
            //         default: break;
            //     }
            //     cout << endl;
            // }
            return data;
        }
    }
}
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
    cout << "touch pos: " << pos.x() << " " << pos.y() << endl;
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

// Direction TouchEvent::analyseDirection(Point start, Point end)
// {
//     // 如果水平方向的偏移>垂直方向的偏移，则为左或右
//     if (abs(start.x() - end.x()) > abs(start.y() - end.y()))
//     {
//         if (start.x() > end.x())
//             return Left;
//         return Right;
//     }
//     else
//     {
//         if (start.y() > end.y())
//             return Up;
//         return Down;
//     }
// }

Direction TouchEvent::analyseDirection(Point start, Point end)
{
    const int MIN_SWIPE_DISTANCE = 20; // 最小滑动距离（像素）

    int dx = end.x() - start.x();
    int dy = end.y() - start.y();
    int abs_dx = abs(dx);
    int abs_dy = abs(dy);

    // 判断总位移是否达到阈值
    if (abs_dx < MIN_SWIPE_DISTANCE && abs_dy < MIN_SWIPE_DISTANCE) {
        cout << "swipe too short" << endl;
        return None; // 滑动太短，视为无效
    }

    // 主要方向判断：x方向变化更大 → 左右；y方向变化更大 → 上下
    if (abs_dx > abs_dy) {
        cout << "swipe direction: " << ((dx > 0) ? "Right" : "Left") << endl;
        return (dx > 0) ? Right : Left;
    } else {
        cout << "swipe direction: " << ((dy > 0) ? "Down" : "Up") << endl;
        return (dy > 0) ? Down : Up;
    }
}
