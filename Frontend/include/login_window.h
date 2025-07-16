#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H


#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QDialog>
#include <vector>

#include "client.hpp"
#include "task.h" 

// 前向声明
class Register_Window;

// 登陆窗口

class Login_Window : public QMainWindow
{
    Q_OBJECT

public:
    Login_Window(client *networkClient,QWidget *parent=nullptr);
    ~Login_Window();

signals:
    void loginSuccessful(const QString &username);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QHBoxLayout *buttonLayout;
    QPushButton *loginButton;
    QPushButton *registerButton;

    client* networkClient; 

    void sendLoginData(const QString &username, const QString &password);
};
#endif // LOGIN_WINDOW_H    