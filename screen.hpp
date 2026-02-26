#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "point.hpp"
#include "color.hpp"
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <cstdint>

class Screen
{
public:
    Screen(const char* device);
    ~Screen();
    void clear(uint32_t color);
    void fill_rect(int x, int y, int w, int h, uint32_t color);
    void swap(); // 把 backBuffer 复制到 fb，并等待 VSync
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    
private:
    uint32_t* _backBuffer;  // 离屏缓冲
    int _width, _height;
    int _fbFd;
    uint8_t* _fbMem;
    struct fb_var_screeninfo _varInfo;
    struct fb_fix_screeninfo _fixInfo;
};
#endif