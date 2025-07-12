#ifndef TASK_H
#define TASK_H
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<algorithm>
#include<functional>
#include<vector>
using namespace std;
// hash函数
inline string hash_username(const string &username)
{
    hash<string> hasher;
    return to_string(hasher(username));
}

//Store time values in seconds
time_t to_time_t(int year, int month, int date, int hour, int minute);

class task{
public:
    string taskName;
    time_t startTime;
    int remind_before;
    int task_id;
    task(string taskname, int year, int month, int date, int hour, int minute, int _remind_before = 600, int taskID = 0) : taskName(taskname), remind_before(_remind_before), task_id(taskID){startTime = to_time_t(year, month, date, hour, minute);}
    task() = default;
    static task fromString(const string& str);
    string toString() const;
    void show() const;
    ~task(){}
};

#endif