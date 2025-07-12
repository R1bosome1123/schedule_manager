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
#include "useraccount.h"
#include "task_manager.h"
#include "FLAG.h"

using namespace std;

// 定义全局flag变量
std::atomic<FLAG> flag(WAIT_LOGGED_IN);

int main()
{
    // Initialize variables
    string current_user_name;
    vector<task> current_user_tasks;  
    
  
    // users_manager all_users; 
    
    info_prompt prompter;
    prompter.welcome_message(); 
    
    while(true)
    {
        switch(flag)
        {    
            case WAIT_LOGGED_IN:
                // current_user_name=all_users.system();
                current_user_name=user_system();
                if(current_user_name == "quit")
                {
                    break;
                    break;
                }
                else
                    flag = SUCCESS_LOGGED_IN; 
            
            case SUCCESS_LOGGED_IN:
                
                mutex mtx; 
                task_manager task_manager(current_user_name,current_user_tasks);
                scan scaner(current_user_name);

                
                auto modified_add_task=[&](){call_with_lock([&](function<void(function<void(vector<task>& tasks)>)> lock_access) {
                                                            task_manager.solve_new_task(lock_access);},
                                                             current_user_tasks,
                                                             flag,
                                                             mtx);}; 

                auto modified_scan_task=[&](){call_with_lock([&](function<void(function<void(vector<task>& tasks)>)> lock_access) {
                                                            scaner.scan_due_task(lock_access);},
                                                             current_user_tasks,
                                                             flag,
                                                             mtx);}; 
                
                
                thread t1(modified_add_task); // Start a thread for task management input
                thread t2(modified_scan_task); // Start a thread for task management scan and output
                t1.join(); // Wait for the task management input thread to finish
                t2.join(); // Wait for the task management scan and output thread to finish
                
                prompter.log_out(current_user_name,current_user_tasks); 
                break;
        }
    }
    prompter.quit_program();
    return 0;
}







