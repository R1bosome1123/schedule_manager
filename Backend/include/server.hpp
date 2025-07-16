// server.hpp
#ifndef SERVER_HPP
#define SERVER_HPP
#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "useraccount.h"
#include "task.h"
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>



using boost::asio::ip::tcp;
using namespace boost::asio;
using json = nlohmann::json;

// server类用于处理客户端连接和请求
// 它使用Boost.Asio库来处理异步I/O操作
// 以及OpenSSL库来处理RSA加密和解密操作
// usermanager类用于管理用户账户，包括注册和密码验证
class server {
public:

    // 构造函数，初始化服务器并开始监听指定端口
    // 使用Boost.Asio库的io_context和tcp::acceptor来处理连接
    // arguments:
    // - io: Boost.Asio的io_context对象，用于处理异步I/O
    // - port: 服务器监听的端口号
    server(io_context& io, unsigned short port);

private:
    void start_accept();
    
    //处理新连接
    void handle_accept(const boost::system::error_code& ec, tcp::socket socket);
    
    //处理会话操作
    void session_handler(tcp::socket socket);
    
    //处理登陆请求
    bool handle_login(json& request,tcp::socket& socket);
    
    //处理注册请求
    bool handle_register(json& request, tcp::socket& socket);
    
    //复合集成处理登录和注册请求
    bool handle_login_and_register(tcp::socket& socket, std::string& out_username);
    
    //读取用户任务数据 from socket exect json
    json receive_json(tcp::socket& socket);
    
    //hash加密username作为存储的文件名，保证隐私性
    std::string hash_username(const std::string &username);
    
    //将json数据发送到socket
    void send_json(tcp::socket& socket, const json& response);
    
    //处理任务存储需求
    void handle_request(tcp::socket& socket, const json& request, const std::string& username);
    
    //将任务数据写入到指定的文件中
    void overwrite_dataset(const std::string& username, const json& tasks_array);
    
    //从指定的文件中读取任务数据
    json read_from_dataset(tcp::socket& socket,std::string& out_username);
    
    //记录日志到文件
    void log(const std::string& level, const std::string& message);

    // RSA私钥解密函数,base64解码函数
    std::string rsa_pri_decrypt(const std::string& data, const std::string& private_key_file);
    std::string base64_decode(const std::string& encoded_data);

    private:
    // Boost.Asio的tcp::acceptor对象，用于监听和接受客户端连接
    // usermanager对象用于管理用户账户，包括注册和密码验证
    tcp::acceptor acceptor_;
    usermanager user_manager;
};
#endif // SERVER_HPP
