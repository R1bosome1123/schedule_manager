#include<iostream>
#include <string>
#include <unistd.h> 
#include <atomic>
#include <vector>

#include "info_prompt.h"
#include "task.h"
using namespace std;

void info_prompt::quit_program() const
{
    cout<<"程序已关闭，再见！"<<endl;
    sleep(1); // Sleep for 1 second before exiting
}


void info_prompt::info_port(long port) const 
{
    cout << "服务器正在监听端口 " << port << "...\n";
}

void info_prompt::welcome_message() const
{
    cout<<"日程管理系统的后端数据库已开启！"<<endl;
}



