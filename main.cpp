#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unistd.h> 
#include <thread>
#include <chrono>
#include <atomic>
#include <map>


#include "thread_lock.h"
#include "info_prompt.h"
#include "task.h"

using namespace std;
enum FLAG
{
    WAIT_LOGGED_IN = 0,
    ASSERT_USER_IDENTITY = 1,
    SUCCESS_LOGGED_IN = 2,
};


int main()
{
    //variable which is used together 
    //(& reference parameter or global variable)
    //this is & referencemeter for you to modify the variable in your function
    string current_user_name;
    string current_user_password;
    atomic<FLAG> flag(WAIT_LOGGED_IN); 
    vector<task> current_user_tasks;  //tasks = {string + time_t} wait for He
    
  
    users_manager all_users; 
    all_users.read_user_credentials("/path/to/credentials.txt"); 
    //REMARK:This function reads user credentials from a file and stores them in the map.
    
    welcome_message(); 
    
    while(true)
    {
        switch(flag)
            case WAIT_LOGGED_IN:
            {
            // assert that the user is not logged in
                get_user_credentials(current_user_name, current_user_password);
                if(current_user_name == "quit"&& current_user_password.empty())
                {
                    break;
                    break;
                }
                else 
                    // Set flag to indicate user is logged in
                    Flag = ASSERT_USER_IDENTITY; 
                    break;
            
            case ASSERT_USER_IDENTITY:
                
                if(!all_users.user_login(current_user_name, current_user_password))
                {
                    flag = SUCCESS_LOGGED_IN; // User is successfully logged in
                }
                 else {
                    
                    flag = WAIT_LOGGED_IN; // Reset to wait for valid credentials
                }
                break;
            case SUCCESS_LOGGED_IN:
                auto modified_add_task=call_with_lock(add_task, current_user_tasks,flag); 
                auto modified_scan_task=call_with_lock(scan, current_user_tasks,flag); 
                thread t1(modified_add_task); // Start a thread for task management input
                thread t2(modified_scan_task); // Start a thread for task management scan and output
                t1.join(); // Wait for the task management input thread to finish
                t2.join(); // Wait for the task management scan and output thread to finish
                
                log_out(current_user_name, current_user_password,current_user_tasks); // Log out the user
                /*
                Remark:
                    add_task when if 'quit' is typed, it will set the flag to WAIT_LOGGED_IN
                    scan_task also quits when 'quit' is typed, but it will not set the flag
                */
                break;
            }
            
    }
    quit_program();
    return 0;
}



// Function to log out the user
// clear() need be included in add_task
void log_out(string &user_name, string &user_password, vector<task> &tasks)
{
    cout<<"You have been logged out."<<endl;
    current_user_name.clear();
    current_user_password.clear();
    tasks.clear(); 
}



