#ifndef INFO_PROMPT_H
#define INFO_PROMPT_H   
#include <string>
#include <vector>

#include "task.h"
using namespace std;

class info_prompt
{
public:
    void quit_program();
    
    void info_port(long port) const;

    void welcome_message();

};
#endif