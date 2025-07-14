// client.cpp
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;
using json = nlohmann::json;
using namespace boost::asio;

class Client {
public:
    Client(const std::string& host, const std::string& port)
        : resolver_(io_), socket_(io_) {
        auto endpoints = resolver_.resolve(host, port);
        connect(socket_, endpoints);
        std::cout << "已连接到服务器\n";
    }

    bool login(const std::string& username, const std::string& password) {
        std::string encrypted_password = password; // TODO: 实现加密
        json login_request = {
            {"type", "login"},
            {"username", username},
            {"encry_password", encrypted_password}
        };
        send_json(login_request);

        json login_response = receive_json();
        std::cout << "服务器响应: " << login_response.dump(2) << "\n";
        return login_response["status"] == "ok";
    }

    void add_task(const std::string& user, const std::string& title, const std::string& time_str) {
        json add_task_request = {
            {"type", "add_task"},
            {"user", user},
            {"task", {
                {"title", title},
                {"time", time_str}
            }}
        };
        send_json(add_task_request);

        json task_reply = receive_json();
        std::cout << "服务器响应: " << task_reply.dump(2) << "\n";
    }

private:
    io_context io_;
    tcp::resolver resolver_;
    tcp::socket socket_;

    void send_json(const json& j) {
        std::string msg = j.dump();
        write(socket_, buffer(msg));
    }

    json receive_json() {
        char data[1024];
        boost::system::error_code ec;
        size_t len = socket_.read_some(buffer(data), ec);
        if (ec == boost::asio::error::eof || len == 0) {
            throw std::runtime_error("连接已断开");
        }
        if (ec) throw boost::system::system_error(ec);
        return json::parse(std::string(data, len));
    }
};

int main() {
    try {
        Client client("127.0.0.1", "8888");

        std::string username = "user1";
        std::string password = "123456";

        if (!client.login(username, password)) return 1;

        client.add_task(username, "写代码", "2025-07-13 09:00:00");

    } catch (const std::exception& e) {
        std::cerr << "[客户端错误] " << e.what() << "\n";
    }
    return 0;
}
