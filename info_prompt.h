#ifndef INFO_PROMPT_H
#define INFO_PROMPT_H   
#include <string>
using namespace std;

class info_prompt
{
public:
    void quit_program();
    void get_user_credentials(string &username, string &password);
    void welcome_message();
}
#endif