// server.cpp
#include "server.hpp"

server::server(io_context& io, unsigned short port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void server::start_accept() {
    tcp::socket socket(acceptor_.get_executor().context());
    acceptor_.async_accept(
        socket,
        [this](const boost::system::error_code& ec) mutable {
            handle_accept(ec, std::move(socket));
            start_accept(); // 准备接收下一个连接
        });
}

void server::handle_accept(const boost::system::error_code& ec, tcp::socket socket) {
    if (!ec) {
        std::cout << "[新连接] 来自 " << socket.remote_endpoint() << std::endl;
        std::thread(&server::session_handler, this, std::move(socket)).detach();
    } else {
        std::cerr << "[Accept Error] " << ec.message() << std::endl;
    }
}

void server::session_handler(tcp::socket socket) {
    std::string username;
    if (!handle_login(socket, username)) {
        std::cerr << "用户登录失败，关闭连接\n";
        return;
    }

    std::vector<task> user_tasks;
    std::mutex mtx;
    std::atomic<FLAG> flag = FLAG::SUCCESS_LOGGED_IN;

    std::cout << "用户 " << username << " 登录成功，开始处理任务指令...\n";

    auto handler = [&](std::function<void(std::function<void(std::vector<task>&)>)> lock_access) {
        handle_client(lock_access, std::move(socket));
    };
    call_with_lock(handler, user_tasks, flag, mtx);
}

bool server::handle_login(tcp::socket& socket, std::string& out_username) {
    try {
        char data[1024];
        boost::system::error_code ec;
        size_t len = socket.read_some(buffer(data), ec);

        if (ec == boost::asio::error::eof || len == 0) return false;
        if (ec) throw boost::system::system_error(ec);

        json request = json::parse(std::string(data, len));
        if (request.contains("type") && request["type"] == "login" &&
            request.contains("username") && request.contains("encry_password")) {

            std::string username = request["username"];
            std::string encry_password = request["encry_password"];
            std::string password = all_users.decrypt_password(encry_password);

            if (all_users.authenticate(username, password)) {
                out_username = username;
                json reply = {{"status", "ok"}, {"message", "登录成功"}};
                write(socket, buffer(reply.dump()));
                return true;
            }
        }

        json fail = {{"status", "error"}, {"message", "用户名或密码错误"}};
        write(socket, buffer(fail.dump()));
        return false;

    } catch (std::exception& e) {
        std::cerr << "[登录失败] " << e.what() << std::endl;
        return false;
    }
}

void server::handle_client(std::function<void(std::function<void(std::vector<task>&)>)> lock_access,
                           tcp::socket socket) {
    try {
        while (true) {
            char data[1024];
            boost::system::error_code ec;
            size_t len = socket.read_some(buffer(data), ec);

            if (ec == boost::asio::error::eof) break;
            if (ec) throw boost::system::system_error(ec);

            json request = json::parse(std::string(data, len));
            std::string type = request["type"];

            if (type == "add_task") {
                task new_task;
                new_task.user = request["user"];
                new_task.schedule = request["task"]["title"];
                new_task.time = parse_time(request["task"]["time"]);

                auto add_task_lambda = [&](std::vector<task>& tasks) {
                    add_task(new_task, tasks);
                };
                lock_access(add_task_lambda);

                json reply = {{"status", "ok"}, {"message", "任务添加成功"}};
                write(socket, buffer(reply.dump()));
            }

            // TODO: 支持更多操作，如删除、查询任务等
        }
    } catch (std::exception& e) {
        std::cerr << "客户端异常断开: " << e.what() << std::endl;
    }
}


int main() {
    try {
        boost::asio::io_context io_context;
        server srv(io_context, 8888);
        std::cout << "服务器正在监听端口 8888...\n";
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "[Fatal Error] " << e.what() << std::endl;
    }
    return 0;
}