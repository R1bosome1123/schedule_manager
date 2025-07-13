#include "task.h"
#include "useraccount.h"
#include <vector>
#include <iostream>
using namespace std;

// 标准时间转换
time_t to_time_t(int year, int month, int date, int hour, int minute)
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

// 命令行帮助
void show_help()
{
    cout << "Usage:\n"
         << "  myschedule run               # 交互运行\n"
         << "  myschedule addtask <taskname> <year> <month> <day> <hour> <minute> <remind_before>\n"
         << "  myschedule showtask          # 查看全部任务\n";
}

//将string格式日程转化为任务类型
task task::fromString(const string& str) {
        istringstream iss(str);
        string token;
        task t;
    ///输入正确性判断
        try {
            getline(iss, token, '|'); t.task_id = stoi(token);
            getline(iss, t.taskName, '|');
            getline(iss, token, '|'); t.startTime = stoll(token);
            getline(iss, token, '|'); t.remind_before = stoi(token);

            // 基本判断
            if (t.task_id < 0 || t.remind_before < 0) throw std::invalid_argument("Invalid field");

        } catch (std::exception& e) {
            cerr << "任务数据格式有误，已跳过: " << str << endl;
            t.task_id = -1; // 标记非法任务
        }

        return t;
    }
// 显示任务提醒
void task::show() const 
{
    if (task_id !=-1)
        cout << "任务[" << taskName << "] 开始:\t" << ctime(&startTime) << " 提前 " << remind_before / 60 << " 分钟提醒"<<endl<<"任务id:"<<task_id<< endl;
}

//将日程格式化为string以方便存到文件
string task::toString() const {
    return to_string(task_id) + "|" + taskName + "|" + to_string(startTime) + "|" + to_string(remind_before); 
}