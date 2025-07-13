#include<iostream>
#include <string>
#include <unistd.h> 
#include <atomic>
#include <vector>

#include "info_prompt.h"
#include "task.h"
using namespace std;

void info_prompt::quit_program()
{
    cout<<"程序已关闭，再见！"<<endl;
    sleep(1); // Sleep for 1 second before exiting
}


void info_prompt::log_out(string &user_name , vector<task> &tasks)
{
    cout<<"您已退出登陆！"<<endl<<endl;
    user_name.clear();
    tasks.clear(); 
}


void info_prompt::welcome_message()
{
    cout<<"欢迎来到日程管理系统！"<<endl;
    // cout<<"Please enter your credentials to continue."<<endl;
    // cout<<"Type 'quit' to exit the program."<<endl;
}

