#include "server.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
server::server(io_context& io, unsigned short port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

void server::start_accept() {
    //创建初始化连接的socket
    // 使用shared_ptr来管理socket的生命周期
    // 这样可以确保在异步操作完成后，socket仍然有效
    // 并且在会话结束后自动释放资源
    auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());
    
    // 异步接受连接
    // acceptor_.async_accept() 会在有新连接时调用 handle_accept()
    // 这里使用 lambda 表达式来捕获 this 指针和 socket
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
        // 启动一个新的线程来处理当前连接的新会话
        std::thread(&server::session_handler, this, std::move(socket)).detach();
    } else {
        std::cerr << "[Accept Error] " << ec.message() << std::endl;
    }
}

void server::session_handler(tcp::socket socket) {
    std::string username;
    // 处理登录或注册请求
    if (!handle_login_and_register(socket, username)) {
        std::cerr << "用户登录失败，关闭连接\n";
        return;
    }
    // 等待处理会话操作
    try {
        while (true) {
            json request = receive_json(socket);
            if (request.is_null()) break; // 客户端断开连接
            log("info", "收到来自用户 " + username + " 的请求: " + request.dump());
            // 处理请求
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
        out_username = request.contains("user_name") ? request["user_name"] : "";   
        
        //case: 如果请求类型是登录
        if (request.contains("type") && request["type"] == "login" &&
            request.contains("user_name") && request.contains("user_password")) {
            return handle_login(request, socket);
        }

        //case: 如果请求类型是注册
        if (request.contains("type") && request["type"] == "register") {
            return handle_register(request, socket);
        }

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
    // 使用私钥解密密码
    std::string user_encrypt_password = request["user_password"];
    std::string user_password = rsa_pri_decrypt(user_encrypt_password, "./key/private.pem");
    //cout<<"[DEBUG] 处理注册请求: 用户名 = " << user_name << ", 用户密码 = " << user_password << endl;
    log("info", "处理登录请求: 用户名 = " + user_name);
    log("info", "处理登录请求: 用户密码 = " + user_encrypt_password);
    // 检查用户名和密码是否匹配
    if (user_manager.password_verify(user_manager.get_user_map()[user_name], user_password)) 
    {

    // 登录成功，读取用户任务数据
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
    // 使用私钥解密密码
    std::string user_password = rsa_pri_decrypt(user_encrypt_password, "./key/private.pem");
    cout<<"[DEBUG] 处理注册请求: 用户名 = " << user_name << ", 用户密码 = " << user_password << endl;
    auto user_map = user_manager.get_user_map();
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
        // 用户名已存在，注册失败
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
        //cout<<"[DEBUG] 读取任务: " << task.dump() << endl; // 调试信息
    }
    file.close();
    log("info", "用户 " + user_name + " 的任务数据已加载成功");
    return json{{"tasks_status", "ok"}, 
                {"tasks_message", "历史任务加载成功"},
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
    // 检查请求类型

    // case : 非法request
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
    
    // case : 退出后的存储任务
    if (type == "send_tasks" && request.contains("tasks")) {
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
     if(tasks_array.empty()) {
        std::cerr << "任务数组为空，未写入任何数据" << std::endl;
        log("warning", "任务数组为空，未写入任何数据");
        return;
    }
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


std::string server::rsa_pri_decrypt(const std::string& encrypted_data, const std::string& private_key_file) {
    std::string decrypted_data;
    
    log("info", "尝试解密数据，原始数据长度: " + std::to_string(encrypted_data.size()));
    
    // 检查数据是否为空
    if (encrypted_data.empty()) {
        log("error", "加密数据为空");
        return "";
    }
    
    // 先尝试Base64解码输入数据
    std::string decoded_data = base64_decode(encrypted_data);
    if (decoded_data.empty()) {
        log("info", "Base64解码失败或数据为空,使用原始数据");
        decoded_data = encrypted_data;
    } else {
        log("info", "Base64解码成功,解码后数据长度: " + std::to_string(decoded_data.size()));
    }

    // 读取私钥文件
    BIO* bio = BIO_new_file(private_key_file.c_str(), "r");
    if (!bio) {
        log("error", "无法打开私钥文件: " + private_key_file);
        return "";
    }

    RSA* rsa = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    if (!rsa) {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        log("error", "读取私钥失败: " + std::string(err_buf));
        return "";
    }

    int rsa_size = RSA_size(rsa);
    log("info", "RSA密钥长度: " + std::to_string(rsa_size) + " 字节");
    
    // 检查数据长度是否合理
    if (decoded_data.size() > static_cast<size_t>(rsa_size)) {
        log("error", "输入数据长度(" + std::to_string(decoded_data.size()) + 
            ")超过RSA密钥长度(" + std::to_string(rsa_size) + ")，数据可能已损坏");
        RSA_free(rsa);
        return "";
    }
    
    decrypted_data.resize(rsa_size);  // 最大可能长度

    int result = RSA_private_decrypt(decoded_data.size(),
                                     reinterpret_cast<const unsigned char*>(decoded_data.data()),
                                     reinterpret_cast<unsigned char*>(&decrypted_data[0]),
                                     rsa, RSA_PKCS1_PADDING);
    RSA_free(rsa);

    if (result == -1) {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        log("error", "RSA 解密失败: " + std::string(err_buf));
        return "";
    }

    decrypted_data.resize(result);  // 实际长度
    log("info", "数据解密成功");
    return decrypted_data;
}




// Base64解码函数
std::string server::base64_decode(const std::string& encoded_data) {
    BIO* bio = BIO_new_mem_buf(encoded_data.data(), encoded_data.size());
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_push(b64, bio);
    
    std::string decoded_data;
    decoded_data.resize(encoded_data.size() * 3 / 4); // Base64解码后的最大长度
    
    int len = BIO_read(bio, &decoded_data[0], decoded_data.size());
    BIO_free_all(bio);
    
    if (len <= 0) {
        return "";
    }
    
    decoded_data.resize(len);
    return decoded_data;
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
