#include "screen.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdexcept>
#include <string.h>
#include <errno.h>

// 构造函数定义
Screen::Screen(const char *name) : _name(name), _w(800), _h(480), _bpp(32)
{
    _fd = ::open(_name, O_RDWR);
    if (_fd == -1)
        throw std::runtime_error(std::string("open fb0 error: ") + strerror(errno));

    // 映射
    _addr = (int *)mmap(NULL, _w * _h * _bpp / 8, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
    if (_addr == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }
    printf("fb0 init success!\n");
}

// 拷贝构造函数定义
Screen::Screen(const Screen &rhs)
{
    this->_name = rhs._name;
    this->_w = rhs._w;
    this->_h = rhs._h;
    this->_bpp = rhs._bpp;

    _fd = ::open(_name, O_RDWR);
    if (_fd == -1)
        throw std::runtime_error(std::string("open fb0 error: ") + strerror(errno));

    // 映射
    _addr = (int *)mmap(NULL, _w * _h * _bpp / 8, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
    if (_addr == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }
    printf("copy fb0 success!\n");
}

// 析构函数定义
Screen::~Screen()
{
    munmap(_addr, _w * _h * _bpp / 8);
    ::close(_fd);
}

void Screen::draw_point(int x, int y, int color) const
{
    // printf("addr = %p \n", _addr);
    if (x >= 0 and x < _w and y >= 0 and y < _h)
    {
        *(_addr + _w * y + x) = color;
    }
}

void Screen::draw_point(const Point &pos, const Color &color) const
{
    draw_point(pos.x(), pos.y(), color.value());
}

void Screen::clear(int color)
{
    for (int y = 0; y < _h; y++) {
        for (int x = 0; x < _w; x++) {
            *(_addr + _w * y + x) = color;
        }
    }
}

void Screen::fill_rect(int x, int y, int w, int h, int color)
{
    for (int dy = y; dy < y + h && dy < _h; dy++) {
        for (int dx = x; dx < x + w && dx < _w; dx++) {
            *(_addr + _w * dy + dx) = color;
        }
    }
}
