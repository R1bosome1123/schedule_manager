// server.hpp
#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "task.hpp"     // 包含 task 定义
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

    tcp::acceptor acceptor_;
};

// 登录处理函数
bool handle_login(tcp::socket& socket, std::string& out_username);

// 处理客户端连接
void handle_client(std::function<void(std::function<void(std::vector<task>&)>)> lock_access,
                   tcp::socket socket);
                   