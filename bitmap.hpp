#ifndef BITMAP_HPP
#define BITMAP_HPP


#include <string>
#include "screen.hpp"

// 假设是24位位图
class Bitmap
{
public:
    Bitmap(std::string name);
    ~Bitmap();
    void draw(Screen& s) const;

private:
    std::string _name;
    int _fd;
    int _w;
    int _h;
    int _size;    
    char* _pix_data;
};

#endif