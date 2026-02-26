#include "shape.hpp"
#include <cmath>
#include <cstdlib>
#include <algorithm>

void Shape::drawCircle(Screen& screen, int x, int y, int radius, const Color& color) {
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = static_cast<int>(sqrt(radius * radius - dy * dy));
        screen.fill_rect(x - dx, y + dy, 2 * dx + 1, 1, color.value());
    }
}

void Shape::drawTriangle(Screen& screen, int x1, int y1, int x2, int y2, int x3, int y3, const Color& color) {
    auto drawLine = [&](int x1, int y1, int x2, int y2) {
        int dx = std::abs(x2 - x1);
        int sx = x1 < x2 ? 1 : -1;
        int dy = -std::abs(y2 - y1);
        int sy = y1 < y2 ? 1 : -1;
        int err = dx + dy;
        int e2;

        while (true) {
            screen.fill_rect(x1, y1, 1, 1, color.value());
            if (x1 == x2 && y1 == y2) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x1 += sx; }
            if (e2 <= dx) { err += dx; y1 += sy; }
        }
    };

    // 绘制三角形边框
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);

    // 简单填充算法
    int minX = std::min({x1, x2, x3});
    int maxX = std::max({x1, x2, x3});
    int minY = std::min({y1, y2, y3});
    int maxY = std::max({y1, y2, y3});

    for (int py = minY; py <= maxY; py++) {
        for (int px = minX; px <= maxX; px++) {
            // 简单点是否在三角形内的判断
            auto sign = [](int x1, int y1, int x2, int y2, int x3, int y3) {
                return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
            };

            float d1 = sign(px, py, x1, y1, x2, y2);
            float d2 = sign(px, py, x2, y2, x3, y3);
            float d3 = sign(px, py, x3, y3, x1, y1);

            bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
            bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

            if (!(has_neg && has_pos)) {
                screen.fill_rect(px, py, 1, 1, color.value());
            }
        }
    }
}

void Shape::drawRect(Screen& screen, int x, int y, int width, int height, const Color& color) {
    screen.fill_rect(x, y, width, height, color.value());
}
