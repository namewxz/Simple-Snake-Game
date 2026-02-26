#include "bitmap.hpp"
#include "color.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

Bitmap::Bitmap(std::string name) : _name(name)
{
    _fd = open(_name.c_str(), O_RDONLY);
    if (_fd == -1)
    {
        perror("open bmp error");
        exit(1);
    }

    // 判断是不是bmp图像
    char bm[2] = {0};
    read(_fd, bm, 2);
    if (bm[0] != 'B' || bm[1] != 'M')
    {
        printf("this is not a bitmap!\n");
        exit(1);
    }
    // 读属性数据
    // 读文件大小
    int size;
    read(_fd, &size, 4);
    // 读文件的宽和高

    lseek(_fd, 0x12, SEEK_SET);
    read(_fd, &_w, 4);
    read(_fd, &_h, 4);

    // 读像素字节
    _pix_data = new char[size-54];
    lseek(_fd, 0x36, SEEK_SET);
    read(_fd, _pix_data, size - 54);
}

Bitmap::~Bitmap()
{
    // 不再释放像素内存，由Screen管理
    // 关闭文件
    close(_fd);
}

void Bitmap::draw(Screen& s) const
{
    // 计算每行的填充字节数
    int pad_bytes = (_w * 24 / 8 % 4 == 0) ? 0 : (4 - _w * 24 / 8 % 4); // 0,1,2,3

    char *p = _pix_data;
    char r, g, b;
    // 绘制
    for (int i = _h - 1; i >= 0; i--)
    {
        for (int j = 0; j < _w; j++)
        {
            b = *p++;
            g = *p++;
            r = *p++;
            Color pixelColor(r, g, b);
            s.fill_rect(j, i, 1, 1, pixelColor.value());
        }
        // 处理完一行的像素字节后，跳过后面的填充字节
        p += pad_bytes;
    }
}
