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


class client : public QObject {
    Q_OBJECT
private:
    QTcpSocket* tcpSocket;
    QString serverIP;
    quint16 serverPort;
    bool connected = false;

public:
    client(QObject* parent = nullptr);
    void setServerInfo(const QString& ip, quint16 port);
    bool connectToServer(QString& errMsg);
    void closeConnection();
    
    bool loginAndFetchTasks(const QString& username, const QString& password, std::vector<task>& tasks, QString& message);
    bool registerUser(const QString& username, const QString& password, QString& message);
    bool sendAllTasks(const std::vector<task>& tasks, QString& message);
private:
    void log(const std::string& level, const std::string& message);
    string rsa_pub_encrypt(const string& data, const string& public_key);
};
#endif // CLIENT_HPP
