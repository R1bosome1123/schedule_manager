#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unistd.h> 
#include <thread>
#include <chrono>
#include <atomic>
#include <map>
#include <mutex>

#include "thread_lock.h"
#include "info_prompt.h"
#include "scan.h"
#include "task.h"

using namespace std;
enum FLAG
{
    WAIT_LOGGED_IN = 0,
    SUCCESS_LOGGED_IN = 1,
};


int main()
{
    // Initialize variables
    string current_user_name;
    atomic<FLAG> flag(WAIT_LOGGED_IN); 
    vector<task> current_user_tasks;  
    
  
    users_manager all_users; 
    
    
    info_prompt::welcome_message(); 
    
    while(true)
    {
        switch(flag)
            case WAIT_LOGGED_IN:
                current_user_name=all_users.system()
                if(current_user_name == 'quit')
                {
                    break;
                    break;
                }
                else
                    flag = SUCCESS_LOGGED_IN; 
            
            case SUCCESS_LOGGED_IN:
                mutex mtx; 
                class HE(current_user_name);
                scan scaner(current_user_name);
                auto modified_add_task=[&](){call_with_lock(HE.add_task, current_user_tasks,flag,mtx)}; 
                auto modified_scan_task=[&](){call_with_lock(scaner.scan_due_task, current_user_tasks,flag,mtx)}; 
                thread t1(modified_add_task); // Start a thread for task management input
                thread t2(modified_scan_task); // Start a thread for task management scan and output
                t1.join(); // Wait for the task management input thread to finish
                t2.join(); // Wait for the task management scan and output thread to finish
                
                info_prompt::log_out(current_user_name,current_user_tasks); 
                break;
    }
    info_prompt::quit_program();
    return 0;
}







