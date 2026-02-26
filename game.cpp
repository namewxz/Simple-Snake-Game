#include "game.hpp"
#include "color.hpp"
#include "shape.hpp"
#include "word.hpp"
#include <algorithm>

#include <unistd.h>

Game::Game(Screen& screen) : 
    _screen(screen), 
    _touch(),
    _state(State::READY),
    _running(true),
    _menuBackground("./bmps/snake.bmp") {
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
    _snake.push_back(Point(GAME_WIDTH/2, HEIGHT/2));
    _currentDir = Direction::Right;
    _score = 0;
    _totalScore = 0;
    _foods.clear();
    for (int i = 0; i < _maxFoods; i++) {
        generateFood();
    }
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
        auto touchData = _touch.getTouchEvent();
        
        // 处理方向输入
        if (touchData.dir != Direction::None) {
            std::lock_guard<std::mutex> lock(_mutex);
            handleInput(touchData.dir);
        }

        // 处理按钮点击
        if (touchData.pos.y() >= 428 && touchData.pos.y() <= 458) { // 确保Y轴在按钮范围内
            std::lock_guard<std::mutex> lock(_mutex);
            
            // 按钮1: 增加速度 (611,428)-(641,458)
            if (touchData.pos.x() >= 611 && touchData.pos.x() <= 641) {
                _speed -= 20000; // 减少20ms
                if (_speed < 50000) _speed = 50000; // 最小50ms
                printf("Speed increased to %d ms\n", _speed/1000);
            }
            // 按钮2: 减少速度 (659,428)-(689,458)
            else if (touchData.pos.x() >= 659 && touchData.pos.x() <= 689) {
                _speed += 20000; // 增加20ms
                if (_speed > 300000) _speed = 300000; // 最大300ms
                printf("Speed decreased to %d ms\n", _speed/1000);
            }
            // 按钮3: 增加食物 (709,428)-(739,458)
            else if (touchData.pos.x() >= 709 && touchData.pos.x() <= 739) {
                if (_maxFoods < 10) {
                    _maxFoods++;
                    // 立即补充一个新食物
                    generateFood();
                    printf("Food increased to %d\n", _maxFoods);
                }
            }
            // 按钮4: 减少食物 (757,428)-(787,458)
            else if (touchData.pos.x() >= 757 && touchData.pos.x() <= 787) {
                if (_maxFoods > 1) {
                    _maxFoods--;
                    // 移除最后一个食物
                    if (!_foods.empty()) {
                        _foods.pop_back();
                    }
                    printf("Food decreased to %d\n", _maxFoods);
                }
            }
        }

        usleep(5000); // 5ms间隔提高响应速度
    }
}

void Game::update() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_state != State::PLAYING) return;

    //printf("Updating snake position, current direction: %d\n", static_cast<int>(_currentDir));
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
    auto foodIt = std::find(_foods.begin(), _foods.end(), newHead);
    if (foodIt != _foods.end()) {
        // 计算单个食物得分并累加
        float speedFactor = (300000.0f - _speed) / (300000.0f - 50000.0f) * 2.0f + 1.0f;
        float foodFactor = 10.0f / _foods.size();
        _score = 1 * speedFactor * foodFactor;
        _totalScore += _score;
        
        _foods.erase(foodIt);
        generateFood();
    } else {
        _snake.pop_back();
    }
}

void Game::render() {
    //printf("Rendering frame - snake size: %zu\n", _snake.size());
    _screen.clear(0); // 清屏为黑色

    // 绘制蛇
    Color snakeColor(0, 255, 0); // 绿色
    for (size_t i = 0; i < _snake.size(); i++) {
        if (i == 0) {
            // 蛇头 - 红色圆形
            Color headColor(255, 0, 0);
            Shape::drawCircle(_screen, 
                             _snake[i].x() + GRID_SIZE/2, 
                             _snake[i].y() + GRID_SIZE/2, 
                             GRID_SIZE/2, 
                             headColor);
        } else {
            // 蛇身 - 绿色矩形
            _screen.fill_rect(_snake[i].x(), _snake[i].y(), GRID_SIZE, GRID_SIZE, snakeColor.value());
        }
    }

    // 绘制食物
    Color foodColor(0, 0, 255);
    for (const auto& food : _foods) {
        int x = food.x() + GRID_SIZE/2;
        int y = food.y();
        Shape::drawTriangle(_screen,
                           x, y,
                           x - GRID_SIZE/2, y + GRID_SIZE,
                           x + GRID_SIZE/2, y + GRID_SIZE,
                           foodColor);
    }
    // 绘制菜单背景
    _menuBackground.draw(_screen, GAME_WIDTH, 0);
    
    // 绘制得分区背景 (在按钮上方，y从380到408)
    _screen.fill_rect(611, 380, 176, 35, 0x333333); // 灰色背景
    
    // 显示累计总得分 (使用WordDisplay类)
    printf("Displaying total score: %d\n", _totalScore);
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "%d", _totalScore);
    WordDisplay::displayNumber(_screen, 620, 380, 24, 35, _totalScore, 0x00FF00);
    
    // 绘制菜单按钮 30x30 黄色 (保持原有按钮位置)
    _screen.fill_rect(611, 428, 30, 30, 0xFFFF00);
    _screen.fill_rect(659, 428, 30, 30, 0xFFFF00);
    _screen.fill_rect(709, 428, 30, 30, 0xFFFF00);
    _screen.fill_rect(757, 428, 30, 30, 0xFFFF00);
    // 交换缓冲区
    _screen.swap();

    // 游戏状态提示
    if (_state == State::READY) {
        // 显示开始提示
    } else if (_state == State::GAME_OVER) {
        // 清屏并显示游戏结束图片
        _screen.clear(0);
        Bitmap gameOverBmp("./bmps/my_game_over.bmp");
        gameOverBmp.draw(_screen);
        _screen.swap();
        return; // 不再绘制其他内容
    }
}


void Game::generateFood() {
    if (_foods.size() >= _maxFoods) return;

    Point newFood;
    do {
        newFood.setX((rand() % (GAME_WIDTH/GRID_SIZE)) * GRID_SIZE);
        newFood.setY((rand() % (HEIGHT/GRID_SIZE)) * GRID_SIZE);
    } while (std::find(_snake.begin(), _snake.end(), newFood) != _snake.end() ||
             std::find(_foods.begin(), _foods.end(), newFood) != _foods.end());

    _foods.push_back(newFood);
}

bool Game::checkCollision(const Point& newHead) const {
    // 检查墙壁碰撞
    if (newHead.x() < 0 || newHead.x() >= GAME_WIDTH || 
        newHead.y() < 0 || newHead.y() >= HEIGHT) {
        return true;
    }
    // 检查自身碰撞
    return std::find(_snake.begin(), _snake.end(), newHead) != _snake.end();
}
