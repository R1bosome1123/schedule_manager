#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <thread>
#include <functional>

using boost::asio::ip::tcp;
using json = nlohmann::json;
using namespace boost::asio;

class AsyncClient {
public:
    using Callback = std::function<void(const json&, const boost::system::error_code&)>;

    AsyncClient(const std::string& host, const std::string& port)
        : resolver_(io_), socket_(io_), work_guard_(make_work_guard(io_)) 
    {
        // 启动io_context事件循环线程
        io_thread_ = std::thread([this]() { io_.run(); });

        // 异步解析并连接
        resolver_.async_resolve(host, port,
            [this](const boost::system::error_code& ec, tcp::resolver::results_type endpoints) {
                if (!ec) {
                    async_connect(socket_, endpoints,
                        [this](const boost::system::error_code& ec, const tcp::endpoint&) {
                            if (ec) {
                                std::cerr << "连接失败: " << ec.message() << "\n";
                            } else {
                                std::cout << "已连接到服务器\n";
                                doRead();  // 开始读响应
                            }
                        });
                } else {
                    std::cerr << "解析失败: " << ec.message() << "\n";
                }
            });
    }

    ~AsyncClient() {
        work_guard_.reset();
        io_.stop();
        if (io_thread_.joinable()) io_thread_.join();
    }

    // 异步登录接口，回调返回服务器响应json和错误码
    void login(const std::string& username, const std::string& password, Callback cb) {
        json login_request = {
            {"type", "login"},
            {"username", username},
            {"encry_password", password} // TODO 加密
        };
        sendRequest(login_request, std::move(cb));
    }

    // 异步添加任务接口
    void add_task(const std::string& user, const std::string& title, const std::string& time_str, Callback cb) {
        json add_task_request = {
            {"type", "add_task"},
            {"user", user},
            {"task", { {"title", title}, {"time", time_str} }}
        };
        sendRequest(add_task_request, std::move(cb));
    }

private:
    io_context io_;
    tcp::resolver resolver_;
    tcp::socket socket_;
    std::thread io_thread_;
    executor_work_guard<io_context::executor_type> work_guard_;

    std::string write_buffer_;
    std::string read_buffer_;

    Callback current_callback_;

    // 将请求json写入socket，并保存回调等待响应
    void sendRequest(const json& j, Callback cb) {
        write_buffer_ = j.dump();

        // 给响应设置回调，sendRequest顺序调用要等前一个响应完毕
        current_callback_ = std::move(cb);

        async_write(socket_, buffer(write_buffer_),
            [this](boost::system::error_code ec, std::size_t /*length*/) {
                if (ec) {
                    if (current_callback_) current_callback_({}, ec);
                    current_callback_ = nullptr;
                }
                // 写完后，等待 doRead() 处理读取响应
            });
    }

    // 持续读取服务器响应
    void doRead() {
        async_read_until(socket_, boost::asio::dynamic_buffer(read_buffer_), '\n',
            [this](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string resp_str = read_buffer_.substr(0, length);
                    read_buffer_.erase(0, length);

                    try {
                        auto resp_json = json::parse(resp_str);
                        if (current_callback_) {
                            current_callback_(resp_json, {});
                        }
                    } catch (...) {
                        if (current_callback_) {
                            current_callback_({}, boost::asio::error::invalid_argument);
                        }
                    }
                    current_callback_ = nullptr;

                    // 继续等待下一个响应
                    doRead();
                } else {
                    if (current_callback_) current_callback_({}, ec);
                    current_callback_ = nullptr;
                }
            });
    }
};
