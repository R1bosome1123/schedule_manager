// #include "schedule.h"
// #include "useraccount.h"
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


void task_manager::sort_tasks(vector<task>& tasks) {
        sort(tasks.begin(), tasks.end(), [](const task& a, const task& b) {
            return a.get_task_due_time() < b.get_task_due_time(); 
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

task_manager::task_manager(string name,vector<task>& tasks) : user_name(name)
    {
    }

void task_manager::add_task(const task& t,vector<task>& tasks) 
{
    auto it = find_if(tasks.begin(), tasks.end(), [&](const task& existing) {
        return existing.get_task_name() == t.get_task_name() && existing.get_task_due_time() == t.get_task_due_time();
    });
    if (it != tasks.end()) {
        cout << "该任务已存在，添加失败！" << endl;
        return;
    }

    tasks.push_back(t);
    sort_tasks(tasks);
    cout << "任务添加成功" << endl;
}

void task_manager::modify_task(vector<task>& tasks, int task_id, const task& new_task) 
{
    auto it = find_if(tasks.begin(), tasks.end(), [&](const task& t) { return t.get_task_id() == task_id; });
    auto it2 = find_if(tasks.begin(), tasks.end(), [&](const task& existing) {
        return existing.get_task_name() == new_task.get_task_name() && existing.get_task_due_time() == new_task.get_task_due_time();
    });
    if (it != tasks.end()&& it2 == tasks.end()) 
    {
        *it = new_task; 
        cout << "任务已修改。" << endl;
    } 
    else if (it== tasks.end())
    {
        cout << "未找到任务ID为 " << task_id << " 的任务。" << endl;
        cout << "修改失败!" << endl;
    }
    else if( it2 != tasks.end())
    {
        cout << "该任务已存在，修改失败！" << endl;
    }
}

void task_manager::show_tasks(vector<task>& tasks) const 
{
        if(tasks.size() == 0){
            cout << "当前无任务!" << endl;
            return;
        }
        for(const auto &t : tasks){
            t.show();
        }
}

void task_manager::task_info_guide(int &mode) const {
    cout << "\n===== 任务管理系统 =====" << endl;
    cout << "请选择功能：\n";
    cout << " 0 - 新建任务\n";
    cout << " 1 - 查看当前任务\n";
    cout << " 2 - 保存任务并退出\n";
    cout << " 3 - 删除任务\n";
    cout << " 4 - 修改任务\n";
    cout << "请输入数字选择功能：";

    cin >> mode;

    if (cin.fail() || mode < 0 || mode > 4) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "输入错误，已默认为退出(2)" << endl;
        mode = 2;
    }
}
void task_manager::delete_task(vector<task>& tasks, int task_id) 
    {
        auto it = find_if(tasks.begin(), tasks.end(), [&](const task& t) { return t.get_task_id()== task_id; });
        if (it != tasks.end()) 
        {
            tasks.erase(it);
            cout << "任务已删除。" << endl;
        } 
        else 
        {
            cout << "未找到任务ID为 " << task_id << " 的任务。" << endl;
        }
    }


void task_manager::input_change_task(task *t){
    cout << "请输入任务 格式: 任务名 年 月 日 时 分 提前提醒秒" << endl;

    string line;
    cin.ignore();
    getline(cin, line); 

    istringstream iss(line);
    string name;
    int y, m, d, h, min, remind;

    try {
        iss >> name >> y >> m >> d >> h >> min >> remind;
        if (iss.fail()) throw invalid_argument("输入格式错误");

        // 粗略日期范围判断 
        if (y < 2024 || y > 2100) throw invalid_argument("年份应在 2024~2100 之间");
        if (m < 1 || m > 12) throw invalid_argument("月份范围应为 1~12");
        if (d < 1 || d > 31) throw invalid_argument("日期范围应为 1~31");
        if (h < 0 || h > 23) throw invalid_argument("小时范围应为 0~23");
        if (min < 0 || min > 59) throw invalid_argument("分钟范围应为 0~59");
        if (remind < 0) throw invalid_argument("提醒秒不能为负数");

        static int id_counter = 0;   
        

        *t = task(name, "", 
         y, m, d, h, min,  
         0, 0, remind, 0,  
         id_counter++);   

    } 
    catch (exception& e) 
    {
        cerr << "输入格式错误，创建任务失败：" << e.what() << endl;

    }
}
//trytry
void task_manager::change_task(string name,int y,int m,int d,int h,int min,int remind,task *t)
{
    try {// 粗略日期范围判断 
        if (y < 2024 || y > 2100) throw invalid_argument("年份应在 2024~2100 之间");
        if (m < 1 || m > 12) throw invalid_argument("月份范围应为 1~12");
        if (d < 1 || d > 31) throw invalid_argument("日期范围应为 1~31");
        if (h < 0 || h > 23) throw invalid_argument("小时范围应为 0~23");
        if (min < 0 || min > 59) throw invalid_argument("分钟范围应为 0~59");
        if (remind < 0) throw invalid_argument("提醒秒不能为负数");

        static int id_counter = 0;  
        
        *t = task(name, "",  
         y, m, d, h, min,  
         0, 0, remind, 0,  
         id_counter++);    
    } 
    catch (exception& e) {
        cerr << "输入格式错误，创建任务失败：" << e.what() << endl;
    }
}