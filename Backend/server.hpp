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
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>



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
    bool handle_login(json& request,tcp::socket& socket);
    bool handle_register(json& request, tcp::socket& socket);
    bool handle_login_and_register(tcp::socket& socket, std::string& out_username);
    json receive_json(tcp::socket& socket);
    std::string hash_username(const std::string &username);
    void send_json(tcp::socket& socket, const json& response);
    void handle_request(tcp::socket& socket, const json& request, const std::string& username);
    void overwrite_dataset(const std::string& username, const json& tasks_array);
    json read_from_dataset(tcp::socket& socket,std::string& out_username);
    
    void log(const std::string& level, const std::string& message);

    string rsa_pri_decrypt(const string& data, const string& private_key);

    tcp::acceptor acceptor_;
    usermanager user_manager;
};
#endif // SERVER_HPP
