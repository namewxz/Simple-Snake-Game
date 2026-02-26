#include "game.hpp"
#include "screen.hpp"
#include "touch.hpp"
#include <unistd.h>

int main() {
    try {
        // 初始化屏幕和触摸
        Screen screen("/dev/fb0");
        TouchEvent touch;
        Game game(screen);
        // 游戏主循环
        while (true) {
        // 更新游戏状态
        game.update();
        
        // 渲染
        game.render();
        
        // 控制帧率
        usleep(150000); // 150ms (约6.5 FPS)
            
            // 检查游戏结束
            if (game.getState() == Game::State::GAME_OVER) {
                usleep(2000000); // 暂停2秒
                game.init(); // 重新开始
            }
        }
    } catch (const std::exception& e) {
        // 错误处理
        return 1;
    }
    return 0;
}
