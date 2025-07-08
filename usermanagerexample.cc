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
    int user_id = manager.user_login();
    if (user_id != 0) 
    {
        std::cout << "当前用户 ID: " << user_id << std::endl;
    }
    return 0;
}