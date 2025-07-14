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



int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8888));

        std::cout << "服务器正在监听端口 8888...\n";

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);  // 阻塞，直到新客户端连接

            std::cout << "[新连接] 来自 " << socket.remote_endpoint() << "\n";

            // 为每个客户端连接启动一个线程
            std::thread(session_handler, std::move(socket)).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "[Fatal Error] " << e.what() << std::endl;
    }

    return 0;
}


class server {
public:
    server(io_context& io, unsigned short port) : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }   
private:
    void start_accept() {
        tcp::socket socket(acceptor_.get_io_context());
        acceptor_.async_accept(socket,
            std::bind(&server::handle_accept, this, std::placeholders::_1, std::move(socket)));
    }

}

void session_handler(tcp::socket socket) {
    std::string user_name;

    // 1. 登录验证
    if (!handle_login(socket, user_name)) {
        std::cerr << "用户登录失败，关闭连接。\n";
        return;  // 登录失败，直接退出线程
    }
    vector<task> user_tasks(username);  // 当前用户的任务列表
    std::cout << "用户 " << username << " 登录成功，开始处理任务指令...\n";

    // 2. 任务处理循环
    auto handle_clinet_known=[&](function<void(function<void(vector<task>&)>)> lock_access){handle_client(lock_access, std::move(socket));};
    call_with_lock(handle_clinet_known, user_tasks, flag, mtx);
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

        // 使用 nlohmann/json 库解析 JSON 数据
        json request = json::parse(std::string(data, len));

        if (request.contains("type") && request["type"] == "login" &&
            request.contains("username") && request.contains("password")) {

            std::string username = request["username"];
            std::string encry_password = request["encry_password"];

            std::string password = all_users.decrypt_password(encry_password);  // 解密密码

            if (all_users.authenticate(username, password)) {
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
                task new_task;
                new_task=request["task"] // 输入任务信息
                /*
                t.user = request["user"];
                t.schedule = request["task"]["title"];
                t.time = parse_time(request["task"]["time"]);
                */
                auto add_task_known=[&](vector<task>&tasks){add_task(new_task, tasks);};
                lock_access(add_task_known); // 添加任务
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



