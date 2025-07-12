#include "schedule.h"
#include "useraccou#int.h"
#include "task.h"
#include "FLAG.h"
#include "task_manager.h"


#include <iostream>
#include <fstream>
#include "FLAG.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;
void task_manager::solve_new_task(function<void(vector<task>&)> lock_access)
{
        
    int mode;
    task_info_guide(mode); // 显示任务输入提示
    switch (mode)
    {
    case 0:
        task new_task;
        input_change_task(new_task); // 输入任务信息
        auto add_task_known=[&](vector<task>&tasks){add_task(new_task, tasks);};
        lock_access(add_task_known); // 添加任务
        break;
        
    case 1:
        lock_access(show_tasks); // 显示当前任务
        break;

    case 2:
        lock_access(save_tasks); // 保存任务到文件
        Flag= WAIT_LOGGED_IN; // 设置标志位为等待登录
        break;
    }
}


// 简单 hash 函数（在 schedule.h 里也有，建议移过去）
string task_manager::hash_username(const string &username)
{
    hash<string> hasher;
    return to_string(hasher(username));
}

void task_manager::sort_tasks(vector<task>& tasks) {
        sort(tasks.begin(), tasks.end(), [](const task& a, const task& b) {
            return a.startTime < b.startTime;
        });
    }  

// 标准时间转换
time_t task_manager::to_time_t(int year, int month, int date, int hour, int minute)
{
    tm tm_time = {};
    tm_time.tm_year = year - 1900;
    tm_time.tm_mon = month - 1;
    tm_time.tm_mday = date;
    tm_time.tm_hour = hour;
    tm_time.tm_min = minute;
    tm_time.tm_sec = 0;
    return mktime(&tm_time);
}

task_manager::task_manager(string name,vector<task>tasks) : user_name(name)
    {
        filename= hash_username(name) + ".txt"; // 文件名为用户名的哈希值
        load_tasks(tasks); // 加载任务
    }

void task_manager::save_tasks(vector<task>& tasks) {
        ofstream fout(filename, ios::trunc); // 覆盖式输出
        for (const auto& t : tasks) {
            fout << t.toString() << std::endl;
        }
    }

void task_manager::load_tasks(vector<task>& tasks) {
    ifstream file(filename);
    if (!file) {        //判断是否无历史记录
        cout << "无历史任务记录。" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        task t = task::fromString(line);
        // 忽略已过期的
        if (t.startTime > time(nullptr)) {
            tasks.push_back(t);         //////////////////从文件读入并保存到tasks中//////////////////
        }
    }
    sort_tasks(tasks);
}
void task_manager::add_task(const task& t,vector<task>& tasks) 
{
    // 检查是否有相同的 taskName + startTime
    auto it = find_if(tasks.begin(), tasks.end(), [&](const task& existing) {
        return existing.taskName == t.taskName && existing.startTime == t.startTime;
    });
    if (it != tasks.end()) {
        cout << "该任务已存在，添加失败！" << endl;
        return;
    }

    tasks.push_back(t);
    sort_tasks(tasks);
    cout << "任务添加成功" << endl;

    if(tasks.size() >= 5) save_tasks(tasks);//自动保存
}

void task_manager::show_tasks(vector<task>& tasks) const 
{
    if(tasks.size() == 0)
    {
        cout << "当前无任务!" << endl;
        return;
    }
    for(const auto &t : tasks)
    { /////遍历输出/////
        t.show();
    }
}