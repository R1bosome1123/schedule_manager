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

int main(int argc, char* argv[])
{
    if(argc == 2 && string(argv[1])=="run" )
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
                        prompter.quit_program();
                        return 0;
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
    }
    else if(argc == 2 && string(argv[1])=="-h")
    {
        show_help(); 
    }
    else if(argc == 4 && string(argv[3])=="showtask")
    {
        usermanager all_users;
        all_users.user_login(argv[1], argv[2]);
        string user_name = all_users.get_currentuser();
        if (user_name.empty())
        {
            cout << "登录失败，请检查用户名和密码。" << endl;
            return 1;
        }
        vector<task> current_user_tasks;
        task_manager task_manager(user_name, current_user_tasks);
        task_manager.show_tasks(current_user_tasks); // Show tasks
    }
    else if(argc == 5 && string(argv[3]) == "deletetask")
    {
        usermanager all_users;
        all_users.user_login(argv[1], argv[2]);
        string user_name = all_users.get_currentuser();
        if (user_name.empty())
        {
            cout << "登录失败，请检查用户名和密码。" << endl;
            return 1;
        }
        vector<task> current_user_tasks;
        task_manager task_manager(user_name, current_user_tasks);
        int task_id = stoi(argv[4]);
        task_manager.delete_task(current_user_tasks, task_id); // Delete task by ID
        task_manager.save_tasks(current_user_tasks); // Save tasks to file
    }
    else if (argc == 11 && string(argv[3]) == "addtask")
    {
        usermanager all_users;
        all_users.user_login(argv[1], argv[2]);
        string user_name = all_users.get_currentuser();
        if (user_name.empty())
        {
            cout << "登录失败，请检查用户名和密码。" << endl;
            return 1;
        }
        vector<task> current_user_tasks;
        task_manager task_manager(user_name, current_user_tasks);
        task new_task;
        task_manager.change_task(argv[4], stoi(argv[5]), stoi(argv[6]), stoi(argv[7]), stoi(argv[8]), stoi(argv[9]), stoi(argv[10]), &new_task); // Change task details
        task_manager.add_task(new_task, current_user_tasks); // Add new task
        task_manager.save_tasks(current_user_tasks); // Save tasks to file
    }
    else if(argc == 12 && string(argv[3]) == "modifytask")
    {
        usermanager all_users;
        all_users.user_login(argv[1], argv[2]);
        string user_name = all_users.get_currentuser();
        if (user_name.empty())
        {
            cout << "登录失败，请检查用户名和密码。" << endl;
            return 1;
        }
        vector<task> current_user_tasks;
        task_manager task_manager(user_name, current_user_tasks);
        int task_id = stoi(argv[4]);
        task new_task;
        task_manager.change_task(argv[5], stoi(argv[6]), stoi(argv[7]), stoi(argv[8]), stoi(argv[9]), stoi(argv[10]), stoi(argv[11]), &new_task); // Change task details
        task_manager.modify_task(current_user_tasks, task_id, new_task); // Modify existing task
        task_manager.save_tasks(current_user_tasks); // Save tasks to file
        task_manager.show_tasks(current_user_tasks); // Show tasks after modification
    }
    else
    {
        cout << "参数错误，请使用 -h 查看帮助信息。" << endl;
    }
    return 0;
}







