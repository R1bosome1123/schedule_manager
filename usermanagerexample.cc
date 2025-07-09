#include "library.h"
#include "useraccount.h"

int main()
{
    // SetConsoleOutputCP(65001);
    usermanager manager;
    // 示例：注册用户
    if (manager.register_user()) 
    {
        cout << "注册成功！" << std::endl;
    }
    // 示例：用户登录
    manager.user_login();
    string user_name = manager.get_currentuser();
    std::cout << "当前用户名: " << user_name << std::endl;
    return 0;
}