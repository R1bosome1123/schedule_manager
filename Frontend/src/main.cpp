#include <QApplication>
#include "login_window.h"
#include "register_window.h"
#include "mainwindow.h"
// 定义全局flag变量

std::vector<task> tasks; // 存储任务的向量
client* networkClient = new client; // 网络通信客户端
string current_user_name;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    MainWindow *mainWindow = nullptr; // 主窗口指针
    // 设置应用程序信息
    app.setApplicationName("日程管理器");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Schedule Manager Team");
    
    // 创建并显示登录窗口
    Login_Window loginWindow(tasks, networkClient,nullptr);
    loginWindow.show();
    
    QObject::connect(&loginWindow, &Login_Window::loginSuccessful, 
                     [&](const QString &username) {
                        //  打开主界面
                         mainWindow = new MainWindow(tasks, networkClient,nullptr);
                         mainWindow->show();
                         current_user_name = username.toStdString();
                         // 隐藏登录窗口
                         loginWindow.hide();
                     });
                     
    return app.exec();
}
