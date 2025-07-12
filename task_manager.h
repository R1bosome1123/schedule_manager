#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "task.h"

using namespace std;

class task_manager {
private:
    string user_name;
    string filename;
private:
    string hash_username(const string &username);
    time_t to_time_t(int year, int month, int date, int hour, int minute);
    void sort_tasks(vector<task>& tasks);
    void load_tasks(vector<task>& tasks);
    void add_task(const task& t,vector<task>& tasks) ;
    void show_tasks(vector<task>& tasks) const;
    void save_tasks(vector<task>& tasks) ;   
    void task_info_guide(int &mode) const;
public:
    task_manager(string name,vector<task>tasks) ;
    void solve_new_task(function<void(function<void(vector<task>&)>)>& lock_access);
    
};

#endif // TASK_MANAGER_H