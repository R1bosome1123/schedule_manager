#include "library.h"
#include "useraccount.h"

string user_system();

// int main()
// {
//     SetConsoleOutputCP(65001);
    
//     cout<<user_system();
//     return 0;
// }





//这是状态机登录部分的函数，调用时修改函数名称再调用即可

string user_system()
{
    // SetConsoleOutputCP(65001);
    usermanager manager;
    string flag = {};
    while(true)
    {
        cout<<"请登录你的账号！"<<endl;
        cout<<"输入1登录账号"<<endl<<
        "输入2注册账号"<<endl
        <<"输入3退出"<<endl;
        cin>>flag;
        if(flag=="1")
        {
            if(manager.user_login()==0)
                return manager.get_currentuser();
        }
        else if(flag=="2")
        {
            manager.register_user();
        }
        else if(flag=="3")
        {
            return "quit";//退出程序
        }
        else
        {
            cout<<"操作不存在！"<<endl;
        }
    }
}