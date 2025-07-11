#ifndef SCHEDULE_H
#define SCHEDULE_H
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<algorithm>
#include<functional>
#include<vector>
using namespace std;
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

    static task fromString(const string& str) {//将string格式日程转化为任务类型
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


    string toString() const {//将日程格式化为string以方便存到文件
        return to_string(task_id) + "|" + taskName + "|" + to_string(startTime) + "|" + to_string(remind_before); 
    }
    void show() const {
        cout << "任务[" << taskName << "] 开始:\t" << ctime(&startTime) << " 提前 " << remind_before / 60 << " 分钟提醒" << endl;
    }
    
    ~task(){}
};

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
    void solve_new_task(function<void(vector<task>&)> lock_access);
    
};
#endif