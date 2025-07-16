#ifndef USERACCOUNT_H
#define USERACCOUNT_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
using namespace std;

// useraccount.h
// 用户账户管理类，用于处理用户注册和密码验证

class usermanager
{
public:
    // 构造函数，初始化用户管理器,自动从文件载入
    usermanager();

    // 核验密码是否匹配
    bool password_verify(string user_password,string user_password2);
    
    // 注册用户接口
    void register_user(string& user_name, string& user_password);
    
    // 获得user_map的公有接口
    map<string, string> get_user_map()
    {
        return user_map;
    }

private:
    // 存储用户名和密码的映射
    map<string, string> user_map; 

};
#endif // USERACCOUNT_H

