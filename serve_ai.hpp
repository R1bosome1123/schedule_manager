// server.hpp
#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "task.h"     // 包含 task 定义
#include "user_db.hpp"   // 包含 all_users 定义（用于验证）

using boost::asio::ip::tcp;
using namespace boost::asio;
using json = nlohmann::json;


class server {
public:
    server(io_context& io, unsigned short port);

private:
    void start_accept();
    void handle_accept(const boost::system::error_code& ec, tcp::socket socket);
    void session_handler(tcp::socket socket);
    bool handle_login(tcp::socket& socket, std::string& out_username);
    void handle_client(std::function<void(std::function<void(std::vector<task>&)>)> lock_access,
                       tcp::socket socket);

    tcp::acceptor acceptor_;
};

// 包裹式访问函数模板
template<typename Function, typename SharedType>
void call_with_lock(Function task_func, SharedType& shared, std::atomic<FLAG>& flag, std::mutex& mtx) {
    auto lock_access = [&](std::function<void(SharedType&)> access_fn) {
        std::lock_guard<std::mutex> lock(mtx);
        access_fn(shared);
    };

    while (flag == FLAG::SUCCESS_LOGGED_IN) {
        task_func(lock_access);
    }
}
