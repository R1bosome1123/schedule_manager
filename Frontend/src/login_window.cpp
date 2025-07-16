#include "login_window.h"
#include "register_window.h"  

extern std::vector<task> tasks; // 全局任务列表
//
Login_Window::Login_Window(client *networkclient, QWidget *parent)
    : QMainWindow(parent)
{
    
    setWindowTitle("登录");
    
    setFixedSize(500, 400);

    
    centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    
    titleLabel = new QLabel("用户登录", centralWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    
    usernameLabel = new QLabel("用户名:", centralWidget);
    usernameEdit = new QLineEdit(centralWidget);
    usernameEdit->setPlaceholderText("请输入用户名");
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);

    
    passwordLabel = new QLabel("密码:", centralWidget);
    passwordEdit = new QLineEdit(centralWidget);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);

    
    buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);

    loginButton = new QPushButton("登录", centralWidget);
    loginButton->setFixedHeight(40);
    registerButton = new QPushButton("注册", centralWidget);
    registerButton->setFixedHeight(40);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    mainLayout->addLayout(buttonLayout);
    
    
    connect(loginButton, &QPushButton::clicked, this, &Login_Window::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &Login_Window::onRegisterClicked);

    networkClient = networkclient;  
    networkClient->setServerInfo("172.20.10.3",50000);
}


Login_Window::~Login_Window()
{
    delete centralWidget; 
}

// 登录按钮点击事件
void Login_Window::onLoginClicked()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    
    
    if (username.isEmpty() || password.isEmpty()) 
    {
        QMessageBox::warning(this, "输入错误", "用户名和密码不能为空");
        return;
    }
    sendLoginData(username, password);
    
}

// 发送登录数据到服务器
void Login_Window::sendLoginData(const QString &username, const QString &password)
{
    QString msg;
    
    this->networkClient->connectToServer(msg);
    if (networkClient->loginAndFetchTasks(username, password, tasks , msg)) 
    {
        QMessageBox::information(this, "登录成功", msg);
        emit loginSuccessful(username);
    } 
    else 
    {
        QMessageBox::warning(this, "登录失败", msg);
        this->networkClient->closeConnection(); 
    }
    
}

// 注册按钮点击事件
void Login_Window::onRegisterClicked()
{
    Register_Window *registerWindow = new Register_Window(this, networkClient);
    
    if (registerWindow->exec() == QDialog::Accepted) 
    {
        QMessageBox::information(this, "提示", "注册完成，已自动登录");
        emit loginSuccessful(""); 
    }
    
    delete registerWindow;
}
