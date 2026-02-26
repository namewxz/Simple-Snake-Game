#ifndef GAME_HPP
#define GAME_HPP

#include "screen.hpp"
#include "point.hpp"
#include "touch.hpp"
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

private:
    void handleTouchInput();
    void generateFood();
    bool checkCollision(const Point& newHead) const;
    void growSnake();

    Screen& _screen;
    TouchEvent _touch;
    State _state;
    std::vector<Point> _snake;
    Point _food;
    Direction _currentDir;
    int _score;
    std::mutex _mutex;
    std::thread _inputThread;
    bool _running;
    static const int GRID_SIZE = 20;
    static const int WIDTH = 800;
    static const int HEIGHT = 480;
};

#endif
