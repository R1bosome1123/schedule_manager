#include <QApplication>
#include "login_window.h"
#include "register_window.h"
#include "mainwindow.h"


std::vector<task> tasks;
client* networkClient = new client;
string current_user_name;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    MainWindow *mainWindow = nullptr;
    
    app.setApplicationName("日程管理器");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Schedule Manager Team");
    
    
    Login_Window loginWindow(networkClient,nullptr);
    loginWindow.show();
    
    QObject::connect(&loginWindow, &Login_Window::loginSuccessful, 
                     [&](const QString &username) {
                         current_user_name= username.toStdString();
                        
                         mainWindow = new MainWindow(tasks, networkClient,nullptr);
                         mainWindow->show();
                         
                         
                         loginWindow.hide();
                     });
                     
    return app.exec();
}
