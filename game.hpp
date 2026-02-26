#ifndef GAME_HPP
#define GAME_HPP

#include "screen.hpp"
#include "point.hpp"
#include "touch.hpp"
#include "bitmap.hpp"
#include <vector>
#include <cstdlib>
#include <thread>
#include <mutex>

using Direction = Direction;

class Game {
public:
    enum class State { READY, PLAYING, GAME_OVER };

    Game(Screen& screen);
    ~Game();
    
    void init();
    void update();
    void render();
    void handleInput(Direction dir);
    State getState() const { return _state; }
    int getSpeed() const { return _speed; }

private:
    void handleTouchInput();
    void generateFood();
    bool checkCollision(const Point& newHead) const;
    void growSnake();

    Screen& _screen;
    TouchEvent _touch;
    State _state;
    std::vector<Point> _snake;
    std::vector<Point> _foods;
    Direction _currentDir;
    int _score; // 当前食物得分
    int _totalScore = 0; // 累计总得分
    std::mutex _mutex;
    std::thread _inputThread;
    bool _running;
    Bitmap _menuBackground; // 菜单背景图
    int _speed = 150000; // 初始速度 (ms)
    int _maxFoods = 3; // 最大食物数量
    static const int GRID_SIZE = 20;
    static const int GAME_WIDTH = 600; // 游戏区域宽度
    static const int UI_WIDTH = 200;   // UI区域宽度
    static const int WIDTH = GAME_WIDTH + UI_WIDTH; // 总宽度800
    static const int HEIGHT = 480;
};

#endif
