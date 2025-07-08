#include "library.h"
using namespace std;

// user 信息存储：user_name user_password user_id

class usermanager
{
    public:
    usermanager();
    bool register_user();
    bool get_password();
    int user_login();
    int user_login(string user_name,string user_password);

    int get_currentuser()
    {
        return current_user;
    }

    int get_maxid();
    int update_maxid();

    private:
    struct useraccount
    {
        string user_name,user_password;
        int user_id;

        useraccount(string strin) 
        {   
            istringstream iss(strin); 
            iss >> user_name;          
            iss >> user_password;      
            iss >> user_id;            
        }

        useraccount(string user_name_in,string user_password_in,int user_id_in)
        {
            user_name=user_name_in;
            user_password=user_password_in;
            user_id=user_id_in;
        }
    };

    int current_user=0;
    vector<useraccount> userlist;
};