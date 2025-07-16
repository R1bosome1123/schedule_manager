#ifndef USERACCOUNT_H
#define USERACCOUNT_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
using namespace std;

// user 信息存储：user_name user_password 

class usermanager
{
    public:
    usermanager();
    bool password_verify(string user_password,string user_password2);
    void register_user(string& user_name, string& user_password);
    map<string, string> get_user_map()
    {
        return user_map;
    }
    private:
    map<string, string> user_map; // 存储用户名和密码的映射

};
#endif // USERACCOUNT_H

