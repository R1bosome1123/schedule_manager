#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <vector>
#include "task.h"

//client类
//用于处理QT GUI MainWindow与后端服务器的通讯连接
//局域网内Tcp::Socket+json包通讯

class client : public QObject {
    Q_OBJECT
private:
    //连接的Socket
    QTcpSocket* tcpSocket;

    //绑定服务器IP与端口
    QString serverIP;
    quint16 serverPort;

    //connected标志
    bool connected = false;

public:
    client(QObject* parent = nullptr);
    //绑定服务器IP和端口
    void setServerInfo(const QString& ip, quint16 port);

    //连接函数
    bool connectToServer(QString& errMsg);

    //中断连接函数
    void closeConnection();
    
    //登录并从数据库读取已有任务，自动过滤过期任务
    bool loginAndFetchTasks(const QString& username, const QString& password, std::vector<task>& tasks, QString& message);

    //注册用户
    bool registerUser(const QString& username, const QString& password, QString& message);

    //传输所有任务到服务器，并自动中断连接
    bool sendAllTasks(const std::vector<task>& tasks, QString& message);
private:

    //日志文件
    void log(const std::string& level, const std::string& message);

    //利用公钥加密password
    string rsa_pub_encrypt(const string& data, const string& public_key);
};
#endif // CLIENT_HPP
