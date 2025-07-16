#include "login_window.h"
#include "register_window.h"  // 包含注册窗口头文件

//
Login_Window::Login_Window(const std::vector<task>& usertasks, client *networkclient, QWidget *parent)
    : QMainWindow(parent)
{
    userTasks=usertasks;
    // 设置窗口标题
    setWindowTitle("登录");
    // 设置窗口大小
    setFixedSize(500, 400);

    // 创建中央部件和主布局
    centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    // 创建标题
    titleLabel = new QLabel("用户登录", centralWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    // 创建用户名输入框
    usernameLabel = new QLabel("用户名:", centralWidget);
    usernameEdit = new QLineEdit(centralWidget);
    usernameEdit->setPlaceholderText("请输入用户名");
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);

    // 创建密码输入框
    passwordLabel = new QLabel("密码:", centralWidget);
    passwordEdit = new QLineEdit(centralWidget);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);

    // 创建按钮布局和按钮
    buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);

    loginButton = new QPushButton("登录", centralWidget);
    loginButton->setFixedHeight(40);
    registerButton = new QPushButton("注册", centralWidget);
    registerButton->setFixedHeight(40);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    mainLayout->addLayout(buttonLayout);
    
    // 连接按钮信号
    connect(loginButton, &QPushButton::clicked, this, &Login_Window::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &Login_Window::onRegisterClicked);

    networkClient = networkclient;  // 初始化网络通信客户端
    networkClient->setServerInfo("172.20.10.3",50000);
}

Login_Window::~Login_Window()
{
    delete centralWidget; // 清理中央部件
}

void Login_Window::onLoginClicked()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    
    // int result=0;
    // 验证输入
    if (username.isEmpty() || password.isEmpty()) 
    {
        QMessageBox::warning(this, "输入错误", "用户名和密码不能为空");
        return;
    }
    sendLoginData(username, password);
    
}

void Login_Window::sendLoginData(const QString &username, const QString &password)
{
    QString msg;
    
    this->networkClient->connectToServer(msg);
    if (networkClient->loginAndFetchTasks(username, password, this->userTasks, msg)) 
    {
        QMessageBox::information(this, "登录成功", msg);
        emit loginSuccessful(username);
    } 
    else 
    {
        QMessageBox::warning(this, "登录失败", msg);
        // emit loginSuccessful(""); // 发送空用户名表示登录失败
    }
    //调试用的登录信息
    emit loginSuccessful(""); // 发送空用户名表示登录失败

}


void Login_Window::onRegisterClicked()
{
    // 创建注册窗口实例
    Register_Window *registerWindow = new Register_Window(this, networkClient);
    
    // 以模态方式显示注册窗口
    if (registerWindow->exec() == QDialog::Accepted) 
    {
        // 用户完成注册后的处理
        QMessageBox::information(this, "提示", "注册完成，已自动登录");
        emit loginSuccessful(""); 
    }
    
    // 清理内存
    delete registerWindow;
}
