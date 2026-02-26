#ifndef COLOR_HPP
#define COLOR_HPP

class Color
{
public:
    Color(int r, int g, int b);
    void setColor(int r, int g, int b);
    int red() const;
    // 。。。

    /**
     * @brief 把一个颜色对象转成一个32位的整数
     */
    int value() const;
private:
    int _red;
    int _green;
    int _blue;
};


#endif
