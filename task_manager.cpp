// #include "schedule.h"
#include "useraccount.h"
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
void task_manager::solve_new_task(function<void(function<void(vector<task>& tasks)>)> lock_access)
{
        
    int mode;
    task_info_guide(mode); // 显示任务输入提示
    switch (mode)
    {
    case 0:
        {
            task new_task;
            input_change_task(&new_task); // 输入任务信息，传递指针
            auto add_task_known=[&](vector<task>&tasks){add_task(new_task, tasks);};
            lock_access(add_task_known); // 添加任务
            break;
        }
        
    case 1:
        lock_access([&](vector<task>& tasks){this->show_tasks(tasks);}); // 显示当前任务
        break;

    case 2:
        lock_access([&](vector<task>& tasks){this->save_tasks(tasks);}); // 保存任务到文件
        flag = WAIT_LOGGED_IN; // 设置标志位为等待登录
        break;
    }
}


// 简单 hash 函数现在使用全局函数

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
        if(tasks.size() == 0){
            cout << "当前无任务!" << endl;
            return;
        }
        for(const auto &t : tasks){ /////遍历输出/////
            t.show();
        }
}

void task_manager::task_info_guide(int &mode) const {
    cout << "\n===== 任务管理系统 =====" << endl;
    cout << "请选择功能：\n";
    cout << " 0 - 新建任务\n";
    cout << " 1 - 查看当前任务\n";
    cout << " 2 - 保存任务并退出\n";
    cout << "请输入数字选择功能：";

    cin >> mode;

    if (cin.fail() || mode < 0 || mode > 2) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "输入错误，已默认为退出(2)" << endl;
        mode = 2;
    }
}


void task_manager::input_change_task(task *t){
    cout << "请输入任务 格式: 任务名 年 月 日 时 分 提前提醒秒" << endl;

    string line;
    cin.ignore();  // 去除上次 cin 留下的 \n
    getline(cin, line); // 一次性读取整行

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

        static int id_counter = 0;   // 静态变量，每次+1
        *t = task(name, y, m, d, h, min, remind, id_counter++);

    } catch (exception& e) {
        cerr << "输入格式错误，创建任务失败：" << e.what() << endl;
        t->task_id = -1; // 标记为非法任务
    }
}