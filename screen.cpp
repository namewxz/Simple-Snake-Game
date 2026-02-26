#include "screen.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>

Screen::Screen(const char* device) {
    // 打开framebuffer设备
    _fbFd = open(device, O_RDWR);
    if (_fbFd == -1) {
        throw std::runtime_error(std::string("open fb0 error: ") + strerror(errno));
    }

    // 获取屏幕信息
    if (ioctl(_fbFd, FBIOGET_VSCREENINFO, &_varInfo) == -1 ||
        ioctl(_fbFd, FBIOGET_FSCREENINFO, &_fixInfo) == -1) {
        throw std::runtime_error("Failed to get screen info");
    }

    // 调试输出屏幕信息
    printf("Screen info:\n");
    printf("Resolution: %dx%d\n", _varInfo.xres, _varInfo.yres);
    printf("Bits per pixel: %d\n", _varInfo.bits_per_pixel);
    printf("Red offset: %d, length: %d\n", _varInfo.red.offset, _varInfo.red.length);
    printf("Green offset: %d, length: %d\n", _varInfo.green.offset, _varInfo.green.length);
    printf("Blue offset: %d, length: %d\n", _varInfo.blue.offset, _varInfo.blue.length);

    _width = _varInfo.xres;
    _height = _varInfo.yres;

    // 映射framebuffer
    _fbMem = (uint8_t*)mmap(NULL, _fixInfo.smem_len, 
                           PROT_READ | PROT_WRITE, MAP_SHARED, _fbFd, 0);
    if (_fbMem == MAP_FAILED) {
        throw std::runtime_error("mmap failed");
    }

    // 分配backBuffer
    _backBuffer = new uint32_t[_width * _height];
}

Screen::~Screen() {
    delete[] _backBuffer;
    munmap(_fbMem, _fixInfo.smem_len);
    close(_fbFd);
}

void Screen::clear(uint32_t color) {
    for (int i = 0; i < _width * _height; i++) {
        _backBuffer[i] = color;
    }
}

void Screen::fill_rect(int x, int y, int w, int h, uint32_t color) {
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            int px = x + dx;
            int py = y + dy;
            if (px >= 0 && px < _width && py >= 0 && py < _height) {
                _backBuffer[py * _width + px] = color;
            }
        }
    }
}

void Screen::swap() {
    // 等待VSync
    int dummy = 0;
    ioctl(_fbFd, FBIO_WAITFORVSYNC, &dummy);

    // 复制backBuffer到framebuffer
    memcpy(_fbMem, _backBuffer, _width * _height * sizeof(uint32_t));
}