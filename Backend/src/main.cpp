#include "server.hpp"
#include "info_prompt.h"
#include <boost/asio.hpp>
#include <iostream>
#include <csignal>   
#include <atomic>

std::atomic<bool> stop_flag(false); 
info_prompt* global_prompt = new info_prompt(); // 全局信息提示实例

// 信号处理函数
void signal_handler(int signal) {
    if (signal == SIGINT) {
        if (global_prompt) {
            global_prompt->quit_program();  // 打印退出信息
        }
        stop_flag = true;  // 设置退出标志
    }
}

int main() {
    try {

        // 注册 Ctrl+C 信号处理
        std::signal(SIGINT, signal_handler);

        global_prompt->welcome_message();

        boost::asio::io_context io_context;
        server srv(io_context, 50000);
        global_prompt->info_port(50000);

        // 使用 run_for + 检查退出标志，替代 io_context.run()
        while (!stop_flag) {
            io_context.poll();  // 非阻塞处理事件
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

    } catch (std::exception& e) {
        std::cerr << "[Fatal Error] " << e.what() << std::endl;
    }

    return 0;
}