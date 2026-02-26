#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "point.hpp"
#include "color.hpp"

class Screen
{
public:
    Screen(const char* name); // 构造函数声明
    Screen(const Screen& rhs); // 拷贝构造函数
    ~Screen();  // 析构函数声明
    void init();
    void draw_point(int x, int y, int color) const; // drawPoint  DrawPoint
    void draw_point(const Point& pos, const Color& color) const;
    void clear(int color);
    void fill_rect(int x, int y, int w, int h, int color);
    
private:
    const char* _name;
    int _fd;
    int* _addr;
    int _w;
    int _h;
    int _bpp; // 色深
};

#endif

