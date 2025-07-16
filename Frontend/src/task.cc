
#include "task.h"
// #include "useraccount.h"
// #include <vector>
#include <iostream>
#include "uniqueid.h"
using namespace std;

// 标准时间转换
time_t task::to_time_t(int year, int month, int date, int hour, int minute,int second)
{
    tm tm_time = {};
    tm_time.tm_year = year - 1900;
    tm_time.tm_mon = month - 1;
    tm_time.tm_mday = date;
    tm_time.tm_hour = hour;
    tm_time.tm_min = minute;
    tm_time.tm_sec = second;
    return mktime(&tm_time);
}

task::task(string name, string description, 
        int due_year, int due_month, int due_day, int due_hour, int due_minute,
        int remind_before_day, int remind_before_hour, int remind_before_minute, int remind_before_second,
        int id)
        : task_name(name), task_description(description){
        task_due_time = to_time_t(due_year, due_month, due_day, due_hour, due_minute);
        long long delta_remind_before_time = One_day_conversion * remind_before_day +
                                             One_hour_conversion * remind_before_hour +
                                             One_minute_conversion * remind_before_minute +
                                             remind_before_second;
        task_remind_time = task_due_time - static_cast<time_t>(delta_remind_before_time);

        RandomIdGenerator::init(); // 初始化随机数种子
        task_id =  RandomIdGenerator::generate(); // 如果id为-1则生成一个随机ID
    }

// 显示任务提醒
void task::show() const 
{
    if (task_id !=-1)
        cout << "任务[" << get_task_name() <<"]"<< endl;
    cout << "描述: " << get_task_description() << endl;
    cout << "截止时间: " << ctime(&task_due_time);
    cout << "提醒时间: " << ctime(&task_remind_time);
}