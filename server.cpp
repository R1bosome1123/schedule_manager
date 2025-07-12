#include <boost/asio.hpp>    // Boost.Asio 网络库头文件
#include <iostream>          // 标准输入输出
#include <thread>            // 多线程支持
#include <vector>
#include <mutex>             // 互斥锁
#include <atomic>            // 原子变量，用于线程安全标志
#include <functional>       // 函数对象支持
#include <string>            // 字符串处理
#include <ctime>             // 时间处理
#include <boost/system/error_code.hpp> // 错误处理
#include "json.hpp"          // JSON 库（nlohmann/json）
#include "task_logic.h"      // 包含 task 结构体与时间解析函数

using namespace boost::asio;
using ip::tcp;
using json = nlohmann::json;

// 全局任务表，每个任务包括时间、用户名、内容
std::vector<task> tasks;

// 全局互斥锁，用于线程安全访问 tasks
std::mutex tasks_mtx;

// 原子标志位：控制是否停止扫描线程
std::atomic<bool> stop_flag{false};



void session_handler(tcp::socket socket) {
    std::string username;

    // 1. 登录验证
    if (!handle_login(socket, username)) {
        std::cerr << "用户登录失败，关闭连接。\n";
        return;  // 登录失败，直接退出线程
    }

    std::cout << "用户 " << username << " 登录成功，开始处理任务指令...\n";

    // 2. 任务处理循环
    handle_client(std::move(socket), username);  // username 用于记录该用户
}


// 登录处理函数
bool handle_login(tcp::socket& socket, std::string& out_username) {
    try {
        char data[1024];
        boost::system::error_code ec;
        size_t len = socket.read_some(buffer(data), ec);

        if (ec == boost::asio::error::eof || len == 0) {
            return false;  // 客户端断开
        }

        if (ec) throw boost::system::system_error(ec);

        // 解析 JSON 请求
        json request = json::parse(std::string(data, len));

        if (request.contains("type") && request["type"] == "login" &&
            request.contains("username") && request.contains("password")) {

            std::string username = request["username"];
            std::string password = request["password"];

            if (authenticate_user(username, password)) {
                out_username = username;

                json reply = {
                    {"status", "ok"},
                    {"message", "登录成功"}
                };
                write(socket, buffer(reply.dump()));
                return true;
            } else {
                json reply = {
                    {"status", "error"},
                    {"message", "用户名或密码错误"}
                };
                write(socket, buffer(reply.dump()));
                return false;
            }
        } else {
            json reply = {
                {"status", "error"},
                {"message", "登录请求格式错误"}
            };
            write(socket, buffer(reply.dump()));
            return false;
        }

    } catch (std::exception& e) {
        std::cerr << "[登录失败] " << e.what() << std::endl;
        return false;
    }
}






// 处理客户端连接（每个连接对应一个线程）
void handle_client(function<void(vector<task>&)> lock_access ,tcp::socket socket,vector<task>&tasks) {
    try {
        while (true) {
            // 接收客户端发来的数据
            char data[1024];
            boost::system::error_code ec;
            size_t len = socket.read_some(buffer(data), ec);

            // 如果客户端断开连接，退出循环
            if (ec == error::eof) break;

            // 如果发生其他错误，抛出异常
            if (ec) throw boost::system::system_error(ec);

            // 解析客户端发来的 JSON 数据
            json request = json::parse(std::string(data, len));
            std::string type = request["type"];

            // 客户端请求添加任务
            if (type == "add_task") {
                lock_access([&](vector<task> tasks){task_manager.add_task(tasks)};);  // 使用锁访问任务管理器的添加任务方法
                //from pocket

                
                task t;
                t.user = request["user"];
                t.schedule = request["task"]["title"];
                t.time = parse_time(request["task"]["time"]); // 字符串转时间戳

                // 加锁，安全地添加任务
                {
                    std::lock_guard<std::mutex> lock(tasks_mtx);
                    tasks.push_back(t);
                }

                // 构造响应消息并返回客户端
                json reply = {
                    {"status", "ok"},
                    {"message", "任务添加成功"}
                };
                write(socket, buffer(reply.dump()));
            }

            // TODO: 可以在这里扩展更多请求类型，例如删除任务、列出任务等
        }
    } catch (std::exception& e) {
        std::cerr << "客户端异常断开：" << e.what() << "\n";
    }
}

// 每秒检查一次是否有任务到期，输出提醒
void scan_due_tasks() {
    while (!stop_flag) {
        {
            std::lock_guard<std::mutex> lock(tasks_mtx);
            time_t now = time(nullptr);

            // 遍历任务列表，查找时间已到的任务
            auto it = tasks.begin();
            while (it != tasks.end()) {
                if (it->time <= now) {
                    std::cout << "提醒任务: " << it->schedule << " （用户：" << it->user << "）\n";
                    it = tasks.erase(it); // 删除已提醒任务
                } else {
                    ++it;
                }
            }
        }

        // 暂停 1 秒后继续
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    io_context io;

    // 创建 TCP 监听器，监听 12345 端口
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345));
    std::cout << "日程管理服务启动，监听端口 12345...\n";

    // 启动后台线程定时扫描任务（线程 2）
    std::thread scanner(scan_due_tasks);

    // 主线程持续接受新的客户端连接
    while (true) {
        tcp::socket socket(io);
        acceptor.accept(socket);
        std::cout << "新客户端已连接\n";

        // 每个连接创建一个线程处理客户端请求
        std::thread(handle_client, std::move(socket)).detach();
    }

    // 程序收尾，结束扫描线程（通常此处不会到达）
    stop_flag = true;
    scanner.join();

    return 0;
}
