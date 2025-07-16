#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>

#include "client.hpp"  

//注册窗口

class Register_Window : public QDialog
{
    Q_OBJECT

public:
    Register_Window(QWidget *parent = nullptr, client *networkClient = nullptr);  // 修复参数名冲突
    ~Register_Window();

private slots:
    void onRegisterClicked();


private:
    QVBoxLayout *mainLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel1;
    QLineEdit *passwordEdit1;
    QLabel *passwordLabel2;
    QLineEdit *passwordEdit2;
    QPushButton *registerButton;
    QPushButton *cancelButton;  // 修复变量名冲突

    client *networkClient;  // 网络通信客户端实例

};  // 添加缺失的分号
#endif