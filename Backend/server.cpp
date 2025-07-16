#include "server.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <openssl/rsa.h>
server::server(io_context& io, unsigned short port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
    //user_manager = usermanager(); // 初始化用户管理器
}

void server::start_accept() {
    auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());
    acceptor_.async_accept(
        *socket,
        [this, socket](const boost::system::error_code& ec) {
            if (!ec) {
                handle_accept(ec, std::move(*socket));
            }
            start_accept(); // 准备接收下一个连接
        });
}

void server::handle_accept(const boost::system::error_code& ec, tcp::socket socket) {
    if (!ec) {
        std::cout << "[新连接] 来自 " << socket.remote_endpoint() << std::endl;
        log("info", "新连接来自 " + socket.remote_endpoint().address().to_string() + ":" + std::to_string(socket.remote_endpoint().port()));
        std::thread(&server::session_handler, this, std::move(socket)).detach();
    } else {
        std::cerr << "[Accept Error] " << ec.message() << std::endl;
    }
}

void server::session_handler(tcp::socket socket) {
    std::string username;
    if (!handle_login_and_register(socket, username)) {
        std::cerr << "用户登录失败，关闭连接\n";
        return;
    }
    try {
        while (true) {
            json request = receive_json(socket);
            if (request.is_null()) break; // 客户端断开连接
            log("info", "收到来自用户 " + username + " 的请求: " + request.dump());
            handle_request(socket, request, username);
        }
    } catch (std::exception& e) {
        std::cerr << "[会话异常] " << e.what() << std::endl;
        log("error", "会话异常: " + std::string(e.what()));
    }
    std::cout << "用户 " << username << " 退出成功\n";      //bug修复：用户退出即调用完session_handler后,终端打印用户退出信息
    return ;

}

bool server::handle_login_and_register(tcp::socket& socket, std::string& out_username) {
    try {
        json request = receive_json(socket);
        if (request.is_null()) {
            std::cerr << "未能接收到有效的登录或注册请求\n";
            log("error", "未能接收到有效的登录或注册请求");
            return false;
        }
        log("info", "处理登录或注册请求: " + request.dump());
        out_username = request.contains("user_name") ? request["user_name"] : "";   //bug修复： 引用的out_username赋值为request["user_name"]，如果没有user_name则赋值为空字符串
        if (request.contains("type") && request["type"] == "login" &&
            request.contains("user_name") && request.contains("user_password")) {
            return handle_login(request, socket);
        }
        if (request.contains("type") && request["type"] == "register") {
            return handle_register(request, socket);
        }

        // 如果请求格式不正确，返回false
        log("error", "未识别的请求类型或格式错误");
        return false;

    } catch (std::exception& e) {
        std::cerr << "[登录失败] " << e.what() << std::endl;
        log("error", "登录失败: " + std::string(e.what()));
        return false;
    }
}

bool server::handle_login(json& request, tcp::socket& socket)
{

    std::string user_name = request["user_name"];
    std::string user_encrypt_password = request["user_password"];
    std::string user_password = rsa_pri_decrypt(user_encrypt_password, "./key/private.pem");
    log("info", "处理登录请求: 用户名 = " + user_name);
    log("info", "处理登录请求: 用户密码 = " + user_encrypt_password);
    // 检查用户名和密码是否匹配
    if (user_manager.password_verify(user_manager.get_user_map()[user_name], user_password)) 
    {
    json login_situation = {{"log_status", "ok"}, {"log_message", "登录成功"}};
    json tasks_situation= read_from_dataset(socket, user_name);
    json reply = 
        {
        {"login", login_situation},
        {"tasks", tasks_situation}
        };
    write(socket, buffer(reply.dump()));
    log("info", "用户 " + user_name + " 读取传输任务成功");
    return true;
    }
    else 
    {
    json register_situation = {
    {"reg_status", "fail"},
    {"reg_message", "用户名已存在"}
    };
    json reply = { {"register", register_situation} };
    write(socket, buffer(reply.dump()));
    log("warning", "用户登录失败，用户名或密码错误");
    return false;
    }
        
}

bool server::handle_register(json& request, tcp::socket& socket)
{
    
    std::string user_name = request["user_name"];
    std::string user_encrypt_password = request["user_password"];
    log("info", "处理注册请求: 用户名 = " + user_name);
    log("info", "处理注册请求: 用户密码 = " + user_encrypt_password);
    // 使用公钥解密密码
    std::string user_password = rsa_pri_decrypt(user_encrypt_password, "./key/public.pem");
    auto user_map = user_manager.get_user_map();
    for(const auto& pair : user_map) {
        log("info", "已注册用户: " + pair.first);
    }
    if (user_map.find(user_name) == user_map.end())
    {
         // 注册新用户
        user_manager.register_user(user_name, user_password);
            
        json register_situation = {{"reg_status", "ok"}, {"reg_message", "注册成功"}};
        json reply = {{"register", register_situation},
                          {"tasks", json::array()}};    // 初始化任务数组为空
        write(socket, buffer(reply.dump()));
        log("info", "用户 " + user_name + " 注册成功");
        return true;
    } 
    else
    {
        json register_situation = {
        {"reg_status", "fail"},
        {"reg_message", "用户名已存在"}
        };
        json reply = { {"register", register_situation} };
        write(socket, buffer(reply.dump()));
        log("warning", "用户名已存在,无法注册: " + user_name);
        return false;
    }
}

json server::read_from_dataset(tcp::socket& socket,std::string& user_name) {
    std::string filename= "./dataset/" + hash_username(user_name) + ".json";
    std::ifstream file(filename);
    if (!file) 
    {
        std::cerr << "无法打开用户数据文件: " << filename << "无历史记录" <<std::endl;
        log("warning", "无法打开用户数据文件: " + filename + " 无历史记录");
        return json{{"tasks_status", "error"}, {"tasks_message", "无历史记录"}};
    }
    std::string line;
    json tasks = json::array();
    while (std::getline(file, line)) {
        json task = json::parse(line);
        tasks.push_back(task);
    }
    file.close();
    return json{{"task_status", "ok"}, 
                {"task_message", "历史任务加载成功"},
                {"tasks", tasks}};
}

json server::receive_json(tcp::socket& socket) {
    char data[4096];
    boost::system::error_code ec;
    size_t len = socket.read_some(buffer(data), ec);

    if (ec == boost::asio::error::eof || len == 0) return nullptr;
    if (ec) throw boost::system::system_error(ec);

    return json::parse(std::string(data, len));
}

void server::send_json(tcp::socket& socket, const json& response) {
    write(socket, buffer(response.dump()));
}

void server::handle_request(tcp::socket& socket, const json& request, const std::string& username) {
    if (!request.contains("type")) {
        json error = {
            {"status", "error"},
            {"message", "请求缺少 type 字段"}
        };
        send_json(socket, error);
        std::cerr << "请求缺少 type 字段" << std::endl;
        log("error", "请求缺少 type 字段");
        return;
    }

    std::string type = request["type"];
    
    if (type == "save_tasks" && request.contains("tasks")) {
        json tasks_array = request["tasks"];
        overwrite_dataset(username, tasks_array);
        json ack = {
            {"status", "ok"},
            {"message", "任务保存成功"}
        };
        send_json(socket, ack);
    } else {
        json error = {
            {"status", "error"},
            {"message", "不支持的请求类型"}
        };
        send_json(socket, error);
        std::cerr << "不支持的请求类型: " << type << std::endl;
        log("error", "不支持的请求类型: " + type);
    }
}

void server::overwrite_dataset(const std::string& username, const json& tasks_array) {
    std::string filename = "./dataset/" + hash_username(username) + ".json";   //在根目录下运行程序，数据集文件存储在./dataset/下，以"hash_username(username)+.json"命名
    std::ofstream file(filename, std::ios::trunc);  // 覆盖写入
    if (!file) 
    {
        std::cerr << "无法打开用户数据文件: " << filename << std::endl;
        log("error", "无法打开用户数据文件: " + filename);
        return;
    }
    for (size_t i = 0; i < tasks_array.size(); ++i) 
        file << tasks_array[i].dump()<< "\n";  // dump() 默认紧凑格式，单行
    file.close();
    std::cout << "用户 " << username << " 的任务数据已保存到 " << filename << std::endl;
    log("info", "用户 " + username + " 的任务数据已保存到 " + filename);
}

std::string server::hash_username(const std::string &username) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(username));
}


std::string server::rsa_pri_decrypt(const std::string& data, const std::string& private_key) {
    std::string decrypted_data;
    BIO* bio = BIO_new_mem_buf(private_key.data(), private_key.size());
    if (!bio) {
        log("error", "创建 BIO 失败");
        return "";
    }
    RSA* rsa = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    if (!rsa) {
        log("error", "读取密钥失败");
        return "";
    }
    int rsa_size = RSA_size(rsa);
    decrypted_data.resize(rsa_size);
    int result = RSA_private_decrypt(data.size(), reinterpret_cast<const unsigned char*>(data.data()),
                                     reinterpret_cast<unsigned char*>(&decrypted_data[0]), rsa, RSA_PKCS1_PADDING);
    RSA_free(rsa);
    if (result == -1) {
        log("error", "RSA 密钥解密失败");
        return "";
    }
    decrypted_data.resize(result); 
    log("info", "数据解密成功");
    return decrypted_data;
}



void server::log(const std::string& level, const std::string& message) {
    std::ofstream logfile("./log/server.log", std::ios::app); // 在根目录下运行程序，log文件会写入 ./log/server.log
    if (!logfile) {
        std::cerr << "无法打开日志文件 server.log" << std::endl;
        return;
    }

    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    char time_buf[64];
    std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", local_time);

    logfile << "[" << level << "]"
            << "[" << time_buf << "] "
            << message << std::endl;

    logfile.close();
}
