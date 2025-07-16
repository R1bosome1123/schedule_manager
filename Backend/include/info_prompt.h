#ifndef INFO_PROMPT_H
#define INFO_PROMPT_H   
#include <string>
#include <vector>

#include "task.h"
using namespace std;

class info_prompt
{
public:
    //构造函数，绑定扫描端口
    void info_port(long port) const;

    //结束程序的消息提醒
    void quit_program() const;        
    
    //欢迎信息
    void welcome_message() const;

};
#endif