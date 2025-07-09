#include "useraccount.h"
#include "library.h"

bool usermanager::register_user()
{
    char choice;
    while (true) 
    {
        if (get_password())
        {
            cout<<"注册成功!"<<endl;
            return true;
        } 
        cout << "是否继续注册？(y/n): ";
        cin >> choice;
        if (choice != 'y' && choice != 'Y') 
            return false;
    }
}

bool usermanager::get_password()
{
    string user_name={},user_password={},user_password_confirm={};
    cout<<"注册账号"<<endl;
    cout<<"请输入用户名:"<<endl;
    cin>>user_name;
    cout<<"请输入密码:"<<endl;
    cin>>user_password;
    cout<<"请确认密码:"<<endl;
    cin>>user_password_confirm;
    
    //报错情况的考虑

    if(user_password==user_password_confirm)
    {
        //口令加密 等之后加入函数进行修改

        //已完成：将用户名和口令写入,分配ID,识别用户名是否已被注册
        for (const auto& user : userlist) 
        {
            if (user.user_name == user_name) 
            {
                cout << "用户名已被注册，请重试！" <<endl;
                return false;
            }
        }
        ofstream file("account.dat",ios::app);
        
        file<<user_name<<" "<<user_password<<endl;
        userlist.push_back(useraccount(user_name,user_password));
        current_user=user_name;
        return true;
    }
    else
    {
        cout<<"两次密码不匹配,请重试！"<<endl;
        return false;
    }    
}

//登录函数 返回1则表示登录失败，返回0表示登录成功
int usermanager::user_login(string user_name,string user_password)
{
    for (const auto& user : userlist) 
    {
        if (user.user_name == user_name) 
        {
            if (user.user_password == user_password) 
            {
                current_user = user.user_name;
                std::cout << "登入成功!" << std::endl;
                std::cout<<"当前登录用户为"<<user_name<<std::endl;
                return 0;
            } 
            else 
            {
                std::cout << "密码错误!" << std::endl;
                return 1;
            }
        }
    }
    
    cout<<"用户不存在!";
    return 1;
}

//登录的重载函数
int usermanager::user_login()
{
    string user_name={},user_password={};
    cout<<"登录账号"<<endl;
    cout<<"请输入用户名:"<<endl;
    cin>>user_name;
    cout<<"请输入密码:"<<endl;
    cin>>user_password;

    return user_login(user_name,user_password);
}

//usermanager的构造函数
usermanager::usermanager()
{
    ifstream account_file("account.dat");
    string line;
    while (std::getline(account_file, line))
    {
        userlist.push_back(useraccount(line));
    }
}