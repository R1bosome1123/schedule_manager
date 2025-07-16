#include "register_window.h"

Register_Window::Register_Window(QWidget *parent, client *networkClient) : QDialog(parent)
{
    this->networkClient = networkClient;  // 使用传入的网络客户端实例
    setWindowTitle("用户注册");
    setFixedSize(400, 500);
    setModal(true);  // 设置为模态窗口

    // QDialog 使用直接布局，不需要centralWidget
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    usernameLabel = new QLabel("用户名:", this);
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);

    passwordLabel1 = new QLabel("密码:", this);
    passwordEdit1 = new QLineEdit(this);
    passwordEdit1->setPlaceholderText("请输入密码");
    mainLayout->addWidget(passwordLabel1);
    mainLayout->addWidget(passwordEdit1);

    passwordLabel2 = new QLabel("确认密码:", this);
    passwordEdit2 = new QLineEdit(this);
    passwordEdit2->setPlaceholderText("请再次输入密码");
    passwordEdit2->setEchoMode(QLineEdit::Password);  // 设置密码模式
    mainLayout->addWidget(passwordLabel2);
    mainLayout->addWidget(passwordEdit2);

    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    registerButton = new QPushButton("注册", this);
    QPushButton *cancelButton = new QPushButton("取消", this);
    
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // 连接信号和槽
    connect(registerButton, &QPushButton::clicked, this, &Register_Window::onRegisterClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);  // 取消按钮
};

Register_Window::~Register_Window()
{
    
}

void Register_Window::onRegisterClicked()  // 添加返回类型
{
    // 注册逻辑待实现
    QString username = usernameEdit->text().trimmed();
    QString password1 = passwordEdit1->text();
    QString password2 = passwordEdit2->text();

    if (username.isEmpty() || password1.isEmpty() || password2.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名或密码不能为空！");
        return;
    }

    if( password1 != password2) 
    {
        QMessageBox::warning(this, "输入错误", "两次输入的密码不一致！");
        return;
    }

    QString msg;
    networkClient->connectToServer(msg);
    if (networkClient->registerUser(username, password1, msg)) 
    {
        QMessageBox::information(this, "注册成功", msg);
        accept();  // 关闭对话框    
    } 
    else 
    {
        QMessageBox::warning(this, "注册失败", msg);
    }
}