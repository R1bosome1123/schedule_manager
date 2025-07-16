#include "client.hpp"
#include <QMessageBox>
#include <algorithm>
#include <ctime>
#include <fstream>
#include "task.h"
#include<openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>

client::client(QObject* parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
}

void client::setServerInfo(const QString& ip, quint16 port)
{
    serverIP = ip;
    serverPort = port;
}

bool client::loginAndFetchTasks(const QString& username, const QString& password, std::vector<task>& tasks, QString& message)
{
    // 此处不再 connect/disconnect
    QJsonObject jsonObj;
    jsonObj["type"] = "login";
    jsonObj["user_name"] = username;
    // 使用公钥加密密码
    string encrypted_password = rsa_pub_encrypt(password.toStdString(), "./key/public.pem");
    if (encrypted_password.empty()) {
        log("error", "密码加密失败");
        return false;
    }
    QByteArray jsonData = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);

    tcpSocket->write(jsonData);
    tcpSocket->waitForBytesWritten();

    if (!tcpSocket->waitForReadyRead(3000)) {
        message = "服务器未响应登录请求";
        log("error", "服务器未响应登录请求");
        return false;
    }

    QByteArray replyData = tcpSocket->readAll();
    QJsonParseError parseError;
    QJsonDocument replyDoc = QJsonDocument::fromJson(replyData, &parseError);
    if (parseError.error != QJsonParseError::NoError || !replyDoc.isObject()) {
        message = "服务器返回数据解析失败";
        log("error", ("服务器返回数据解析失败: " + parseError.errorString()).toStdString());
        return false;
    }

    QJsonObject reply = replyDoc.object();
    
    // 安全地解析 JSON 对象
    QJsonObject loginObj = reply["login"].toObject();
    QString log_status = loginObj["log_status"].toString();
    message = loginObj["log_message"].toString();

    if (log_status != "ok") 
    {
        log("warning", ("登录失败: " + message).toStdString());
        return false;
    }
    
    QJsonObject tasksObj = reply["tasks"].toObject();
    QString task_status = tasksObj["tasks_status"].toString();
    QString task_message = tasksObj["tasks_message"].toString();

    if (task_status != "ok") {
        message = "登录成功，但无历史任务：" + task_message;
        log("info", ("登录成功，但无历史任务: " + task_message).toStdString());
        tasks.clear();
        return true;
    }

    tasks.clear();
    QJsonArray tasksArray = tasksObj["tasks"].toArray();
    for (const auto& task_json : tasksArray) {
        tasks.push_back(task_from_json(task_json.toObject()));
    }

    time_t now = time(nullptr);
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [now](const task& t) {
        return t.get_task_due_time() < now;
    }), tasks.end());

    std::sort(tasks.begin(), tasks.end(), [](const task& a, const task& b) {
        return a.get_task_due_time() < b.get_task_due_time();
    });
    log("info", "登录成功并加载任务: " + std::to_string(tasks.size()) + " tasks");
    message = "登录成功并加载任务: " + QString::number(tasks.size()) + " tasks";
    return true;
}

bool client::registerUser(const QString& username, const QString& password, QString& message)
{
    QJsonObject jsonObj;
    jsonObj["type"] = "register";
    jsonObj["user_name"] = username;
    // 使用公钥加密密码
    string encrypted_password = rsa_pub_encrypt(password.toStdString(), "./key/public.pem");
    jsonObj["user_password"] = QString::fromStdString(encrypted_password);
    QByteArray jsonData = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);

    tcpSocket->write(jsonData);
    tcpSocket->waitForBytesWritten();

    if (!tcpSocket->waitForReadyRead(3000)) {
        message = "服务器未响应注册请求";
        log("error", "服务器未响应注册请求");
        return false;
    }

    QByteArray replyData = tcpSocket->readAll();
    QJsonParseError parseError;
    QJsonDocument replyDoc = QJsonDocument::fromJson(replyData, &parseError);
    if (parseError.error != QJsonParseError::NoError || !replyDoc.isObject()) {
        message = "服务器返回数据解析失败";
        log("error", ("服务器返回数据解析失败: " + parseError.errorString()).toStdString());
        return false;
    }

    QJsonObject reply = replyDoc.object();
    QJsonObject registerObj = reply["register"].toObject();
    QString reg_status = registerObj["reg_status"].toString();
    message = registerObj["reg_message"].toString();

    if (reg_status != "ok") {
        log("warning", ("注册失败: " + message).toStdString());
        return false;
    }

    log("info", ("注册成功: " + message).toStdString());
    return true;
}



bool client::sendAllTasks(const std::vector<task>& tasks, QString& message)
{
    QJsonObject jsonObj;
    jsonObj["type"] = "send_tasks";
    QJsonArray tasksArray;

    for (const auto& task : tasks) {
        QJsonObject taskObj;
        taskObj["task_name"] = QString::fromStdString(task.get_task_name());
        taskObj["task_description"] = QString::fromStdString(task.get_task_description());
        taskObj["task_due_time"] = static_cast<qint64>(task.get_task_due_time());
        taskObj["task_remind_time"] = static_cast<qint64>(task.get_task_remind_time());
        taskObj["task_id"] = task.get_task_id();
        tasksArray.append(taskObj);
    }

    jsonObj["tasks"] = tasksArray;
    QByteArray jsonData = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);

    tcpSocket->write(jsonData);
    tcpSocket->waitForBytesWritten();

    // 可选：等待服务器回应
    message = "任务已成功发送";
    log("info", "任务已成功发送: " + std::to_string(tasks.size()) + " tasks");
    return true;
}


bool client::connectToServer(QString& errMsg)
{
    if (connected) return true;
    tcpSocket->connectToHost(serverIP, serverPort);
    if (!tcpSocket->waitForConnected(3000)) {
        errMsg = "连接失败：" + tcpSocket->errorString();
        log("error", "连接失败: " + errMsg.toStdString());
        connected = false;
        return false;
    }

    connected = true;
    log("info", "成功连接到服务器: " + serverIP.toStdString() + ":" + std::to_string(serverPort));
    return true;
}

void client::closeConnection()
{
    if (connected) {
        tcpSocket->disconnectFromHost();
        connected = false;
        log("info", "已断开与服务器的连接");
    }
}

void client::log(const std::string& level, const std::string& message)
{
    std::ofstream logfile("./log/client.log", std::ios::app);   // 在根目录下运行程序，log文件会写入 ./log/client.log(相对路径)
    if (!logfile) {
        std::cerr << "无法打开日志文件 client.log" << std::endl;
        return;
    }

    std::time_t now = std::time(nullptr);
    logfile << "[" << std::ctime(&now) << "] [" << level << "] " << message << std::endl;
}

string client::rsa_pub_encrypt(const string& data, const string& public_key)
{
    string encrypted_data;
    BIO* bio = BIO_new_mem_buf(public_key.data(), public_key.size());
    if (!bio) {
        log("error", "创建 BIO 失败");
        return "";
    }
    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    if (!rsa) {
        log("error", "读取公钥失败");
        return "";
    }
    int rsa_size = RSA_size(rsa);
    encrypted_data.resize(rsa_size);
    int result = RSA_public_encrypt(data.size(), reinterpret_cast<const unsigned char*>(data.data()),
                                     reinterpret_cast<unsigned char*>(&encrypted_data[0]), rsa, RSA_PKCS1_PADDING);
    RSA_free(rsa);
    if (result == -1) {
        log("error", "RSA 加密失败: " + std::to_string(ERR_get_error()));
        return "";
    }
    encrypted_data.resize(result); 
    log("info", "数据加密成功");
    return encrypted_data;
}
