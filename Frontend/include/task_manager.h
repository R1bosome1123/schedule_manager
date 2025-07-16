#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "task.h"

using namespace std;

// task_manager类定义
// 用于管理任务的添加、删除、修改和显示等功能

class task_manager 
{
private:
    string user_name;
    string filename;
private:
    time_t to_time_t(int year, int month, int date, int hour, int minute);
    void sort_tasks(vector<task>& tasks);  
    void task_info_guide(int &mode) const;
public:
    task_manager(string name,vector<task>& tasks) ;
    // void solve_new_task(function<void(function<void(vector<task>& tasks)>)> lock_access);
    void input_change_task(task *t);
    void delete_task(vector<task>& tasks, int task_id);
    void modify_task(vector<task>& tasks, int task_id, const task& new_task);
    void add_task(const task& t,vector<task>& tasks) ;
    void show_tasks(vector<task>& tasks) const;
    void change_task(string task_name,int y,int m,int d,int h,int min,int time_before,task* t);
};

#endif // TASK_MANAGER_H