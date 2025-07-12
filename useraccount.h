#include "library.h"
using namespace std;

// user 信息存储：user_name user_password 

class usermanager
{
    public:
    usermanager();
    bool register_user();
    bool get_password();
    int user_login();
    int user_login(string user_name,string user_password);
    bool password_verify(string user_password,string user_password2);

    string get_currentuser()
    {
        return current_user;
    }


    private:
    map<string, string> user_map; // 存储用户名和密码的映射
    
    string current_user={};
};


//这是状态机登录部分的函数，调用时修改函数名称再调用即可,返回字符串
string user_system();