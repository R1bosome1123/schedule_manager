#include "useraccount.h"
#include "library.h"

bool usermanager::register_user()
{
    char choice;
    while (true) 
    {
        if (get_password())
        {
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

        //已完成：将用户名和口令写入
        
        if (user_map.find(user_name) != user_map.end())
        {
            cout << "用户名已被注册，请重试！" <<endl;
            return false;
        }
        ofstream file("account.dat",ios::app);

        //
        char hash[crypto_pwhash_STRBYTES];
        if (crypto_pwhash_str(hash, user_password.c_str(), user_password.length(),
                          crypto_pwhash_OPSLIMIT_INTERACTIVE,
                          crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) 
        {
            throw std::runtime_error("哈希生成失败");
        }

        string user_crypted_password=string(hash);


        file<<user_name<<" "<<user_crypted_password<<endl;
        user_map[user_name] = user_crypted_password; // 存储用户名和密码的映射


        return true;
    }
    else
    {
        cout<<"两次密码不匹配,请重试！"<<endl;
        return false;
    }    
}

//need

//登录函数
int usermanager::user_login(string user_name,string user_password)
{
    if (user_name.empty() || user_password.empty()) 
    {
        cout << "用户名或密码不能为空!" <<endl;
        return 0;
    }
    //检查用户名和密码是否匹配
    if (user_map.find(user_name) != user_map.end())
    {
        if (password_verify(user_map[user_name],user_password))  
        {
            current_user = user_name;
            cout << "登入成功!" << endl<<endl;
            return 1;
        } 
        else 
        {
            cout << "密码错误!" << endl<<endl;
            return 0;
        }
    }
    cout<<"用户不存在!"<<endl<<endl;
    return 0;
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
        istringstream iss(line);
        string user_name, user_password;
        iss >> user_name >> user_password;
        // 创建 useraccount 对象并添加到 userlist 中    
        user_map[user_name] = user_password; // 存储用户名和密码的映射
    }
}

bool usermanager::password_verify(string storedHash,string userInputPassword)
{
    int result = crypto_pwhash_str_verify(
        storedHash.c_str(),        // 存储的哈希值
        userInputPassword.c_str(), // 用户输入的密码
        userInputPassword.size()   // 密码长度
    );

    return (result == 0); // 如果result为0，表示密码正确
}


string user_system()
{
    if (sodium_init() < 0) 
    {
        cerr << "libsodium初始化失败！" << endl;
        return "quit"; // 初始化失败时应终止程序
    }

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
            if(manager.user_login()==1)
            {
                string user_name = manager.get_currentuser();
                return user_name;
            }
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
            cout<<"操作不存在！"<<endl<<endl;
        }
    }
}