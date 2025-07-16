#include "useraccount.h"
#include <iostream>
#include <fstream>         
#include <string>  
#include <sstream>
#include <sodium.h> // libsodium for password hashing


//need

//usermanager的构造函数
usermanager::usermanager()
{
    ifstream account_file("./dataset/account.dat");     // 在根目录下运行程序，用户数据文件存储在./dataset/account.dat下
    if (!account_file.is_open()) {
        std::cerr << "无法打开用户数据文件: account.dat" << std::endl;
        return;
    }
    cout<<"打开了用户数据文件： ./dataset/account.dat" << endl;   //命令行提示
    string line;
    while (std::getline(account_file, line))
    {
        istringstream iss(line);
        string user_name, user_password;
        iss >> user_name >> user_password;
        // 创建 useraccount 对象并添加到 userlist 中    
        user_map[user_name] = user_password; // 存储用户名和密码的映射
    }
    cout<< "初始化载入users信息成功" << endl;   //命令行提示
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

void usermanager::register_user(string& user_name, string& user_password)
{
    // 使用 libsodium 生成密码哈希
    cout<< "正在注册新用户: " << user_name << endl; // 调试信息
    char hashed_password[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(
            hashed_password, 
            user_password.c_str(), 
            user_password.size(), 
            crypto_pwhash_OPSLIMIT_INTERACTIVE, 
            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        throw std::runtime_error("密码哈希生成失败");
    }

    // 将用户名和哈希密码存储到 user_map 中
    user_map[user_name] = string(hashed_password);
    
    // 将新用户信息写入文件
    ofstream account_file("./dataset/account.dat");   // 在根目录下运行程序，用户数据文件存储在./dataset/account.dat下 覆盖写入，避免重复存储
    account_file << user_name << " " << hashed_password << endl;
}