#include "game.hpp"
#include "color.hpp"
#include <algorithm>
#include <unistd.h>

Game::Game(Screen& screen) : 
    _screen(screen), 
    _touch(),
    _state(State::READY),
    _running(true) {
    init();
    _inputThread = std::thread(&Game::handleTouchInput, this);
}

Game::~Game() {
    _running = false;
    if (_inputThread.joinable()) {
        _inputThread.join();
    }
}

void Game::init() {
    std::lock_guard<std::mutex> lock(_mutex);
    _snake.clear();
    _snake.push_back(Point(WIDTH/2, HEIGHT/2));
    _currentDir = Direction::Right;
    _score = 0;
    generateFood();
    _state = State::PLAYING;
    printf("Game initialized, state: PLAYING\n");
}

void Game::handleInput(Direction dir) {
    if ((_currentDir == Direction::Up && dir != Direction::Down) ||
        (_currentDir == Direction::Down && dir != Direction::Up) ||
        (_currentDir == Direction::Left && dir != Direction::Right) ||
        (_currentDir == Direction::Right && dir != Direction::Left)) {
        _currentDir = dir;
    }
}

void Game::handleTouchInput() {
    while (_running) {
        Direction dir = _touch.direction();
        if (dir != Direction::None) {
            std::lock_guard<std::mutex> lock(_mutex);
            handleInput(dir);
        }
        usleep(10000); // 10ms间隔
    }
}

void Game::update() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_state != State::PLAYING) return;

    printf("Updating snake position, current direction: %d\n", static_cast<int>(_currentDir));
    // 自动移动蛇
    Point newHead = _snake.front();
    switch(_currentDir) {
        case Direction::Up: newHead.setY(newHead.y() - GRID_SIZE); break;
        case Direction::Down: newHead.setY(newHead.y() + GRID_SIZE); break;
        case Direction::Left: newHead.setX(newHead.x() - GRID_SIZE); break;
        case Direction::Right: newHead.setX(newHead.x() + GRID_SIZE); break;
        default: break; // 保持原方向
    }

    // 检查边界和碰撞
    if (checkCollision(newHead)) {
        _state = State::GAME_OVER;
        return;
    }

    _snake.insert(_snake.begin(), newHead);

    // 检查是否吃到食物
    if (newHead == _food) {
        _score++;
        generateFood();
    } else {
        _snake.pop_back();
    }
}

void Game::render() {
    printf("Rendering frame - snake size: %zu\n", _snake.size());
    _screen.clear(0); // 清屏为黑色

    // 绘制蛇
    Color snakeColor(0, 255, 0); // 绿色
    for (const auto& segment : _snake) {
        _screen.fill_rect(segment.x(), segment.y(), GRID_SIZE, GRID_SIZE, snakeColor.value());
    }

    // 绘制食物
    Color foodColor(255, 0, 0); // 红色
    _screen.fill_rect(_food.x(), _food.y(), GRID_SIZE, GRID_SIZE, foodColor.value());

    // 交换缓冲区
    _screen.swap();

    // 游戏状态提示
    if (_state == State::READY) {
        // 显示开始提示
    } else if (_state == State::GAME_OVER) {
        // 显示游戏结束
    }
}


void Game::generateFood() {
    do {
        _food.setX((rand() % (WIDTH/GRID_SIZE)) * GRID_SIZE);
        _food.setY((rand() % (HEIGHT/GRID_SIZE)) * GRID_SIZE);
    } while (std::find(_snake.begin(), _snake.end(), _food) != _snake.end());
}

bool Game::checkCollision(const Point& newHead) const {
    // 检查墙壁碰撞
    if (newHead.x() < 0 || newHead.x() >= WIDTH || 
        newHead.y() < 0 || newHead.y() >= HEIGHT) {
        return true;
    }
    // 检查自身碰撞
    return std::find(_snake.begin(), _snake.end(), newHead) != _snake.end();
}
