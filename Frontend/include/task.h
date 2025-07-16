#ifndef TASK_H
#define TASK_H
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<algorithm>
#include<functional>
#include<vector>
#include <QString>
#include <QMetaType>
#include <QJsonObject>
using namespace std;

//task类定义
// 任务类用于存储和管理任务信息
// 包含任务名称、描述、截止时间、提醒时间和任务ID等属性

class task{
private:
    string task_name;
    string task_description;
    time_t task_due_time;
    time_t task_remind_time;
    int task_id;

    enum {
        One_day_conversion = 24 * 60 * 60,
        One_hour_conversion = 60 * 60, 
        One_minute_conversion = 60 
    };

private:
time_t to_time_t(int year, int month, int date, int hour, int minute, int second = 0);

public:
    // 构造函数
    task() : task_due_time(0), task_remind_time(0), task_id(-1) {}
    task(string name, string description, 
        int due_year, int due_month, int due_day, int due_hour, int due_minute,
        int remind_before_day = 0, int remind_before_hour = 0, int remind_before_minute = 0, int remind_before_second = 0,
        int id = -1);
    
    // 获取任务信息
    string get_task_name() const { return task_name; }
    string get_task_description() const { return task_description; }
    time_t get_task_due_time() const { return task_due_time; }
    time_t get_task_remind_time() const { return task_remind_time; }
    int get_task_id() const { return task_id; }
    
    bool operator==(const task& other) const {
        //task_id == other.task_id &&
        return  task_name == other.task_name &&
               task_description == other.task_description &&
               task_due_time == other.task_due_time &&
               task_remind_time == other.task_remind_time;
    }    

   

   
    void show() const;
    ~task(){}

    // 与task与json文件的互转
    friend inline task task_from_json(const QJsonObject& obj);
    friend inline QJsonObject task_to_json(const task& t);
};


inline task task_from_json(const QJsonObject& obj)
{
    task t;
    t.task_name = obj["task_name"].toString().toStdString();
    t.task_description = obj["task_description"].toString().toStdString();
    t.task_due_time = static_cast<time_t>(obj["task_due_time"].toDouble());
    t.task_remind_time = static_cast<time_t>(obj["task_remind_time"].toDouble());
    t.task_id = obj["task_id"].toInt();
    return t;
}

inline QJsonObject task_to_json(const task& t)
{
    QJsonObject obj;
    obj["task_name"] = QString::fromStdString(t.get_task_name());
    obj["task_description"] = QString::fromStdString(t.get_task_description());
    obj["task_due_time"] = static_cast<double>(t.get_task_due_time());
    obj["task_remind_time"] = static_cast<double>(t.get_task_remind_time());
    obj["task_id"] = t.get_task_id();
    return obj;
}
#endif