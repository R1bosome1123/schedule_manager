// 项目结构：
// ├── main.cpp
// ├── mainwindow.h
// ├── mainwindow.cpp
// └── mainwindow.ui

// ------------------------
// main.cpp
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

// ------------------------
// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H

// ------------------------
// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), socket(new QTcpSocket(this)) {
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onSocketError);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLoginClicked() {
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    QJsonObject json;
    json["type"] = "login";
    json["username"] = username;
    json["encry_password"] = password;
    QJsonDocument doc(json);

    socket->connectToHost("127.0.0.1", 8888);
    if (socket->waitForConnected(1000)) {
        socket->write(doc.toJson(QJsonDocument::Compact));
    } else {
        QMessageBox::critical(this, "连接失败", "无法连接到服务器。");
    }
}.

void MainWindow::onReadyRead() {
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        QString status = obj["status"].toString();
        QString message = obj["message"].toString();

        if (status == "ok") {
            QMessageBox::information(this, "登录成功", message);
        } else {
            QMessageBox::warning(this, "登录失败", message);
        }
    }
}

void MainWindow::onSocketError(QAbstractSocket::SocketError) {
    QMessageBox::critical(this, "网络错误", socket->errorString());
}

// ------------------------
// mainwindow.ui
/* 以下是 Qt Designer 生成的 .ui 文件 XML 内容，可以保存为 mainwindow.ui：
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x><y>0</y><width>400</width><height>300</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>登录界面</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <widget class="QLineEdit" name="usernameEdit">
      <property name="placeholderText">
       <string>用户名</string>
      </property>
     </widget>
    </item>
    <item>
     <widget class="QLineEdit" name="passwordEdit">
      <property name="placeholderText">
       <string>密码</string>
      </property>
      <property name="echoMode">
       <enum>QLineEdit::Password</enum>
      </property>
     </widget>
    </item>
    <item>
     <widget class="QPushButton" name="loginButton">
      <property name="text">
       <string>登录</string>
      </property>
     </widget>
    </item>
   </layout>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>
*/
