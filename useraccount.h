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

    string get_currentuser()
    {
        return current_user;
    }


    private:
    struct useraccount
    {
        string user_name,user_password;

        useraccount(string strin) 
        {   
            istringstream iss(strin); 
            iss >> user_name;          
            iss >> user_password;        
        }

        useraccount(string user_name_in,string user_password_in)
        {
            user_name=user_name_in;
            user_password=user_password_in;
        }
    };
    string decrypt_password(const string& encrypted_password);
    string current_user={};
    vector<useraccount> userlist;
};