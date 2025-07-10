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


class TaskManager {
private:
    vector<task> tasks;
    string filename;
    void sortTasks() {
        sort(tasks.begin(), tasks.end(), [](const task& a, const task& b) {
            return a.startTime < b.startTime;
        });
    }

public:
    TaskManager(const string& username){
        filename = hash_username(username);
    }
    void addTask(const task& t){
        /////////////////////////////////////vector//////////////////////////////////////////
        // 检查是否有相同的 taskName + startTime
        auto it = find_if(tasks.begin(), tasks.end(), [&](const task& existing) {
            return existing.taskName == t.taskName && existing.startTime == t.startTime;
        });
        if (it != tasks.end()) {
            cout << "该任务已存在，添加失败！" << endl;
            return;
        }

        tasks.push_back(t);
        sortTasks();
        cout << "任务添加成功" << endl;

        if(tasks.size() >= 5) saveTasks();//自动保存
    }
    void showTasks() const{
        if(tasks.size() == 0){
            cout << "当前无任务!" << endl;
            return;
        }
        for(const auto &t : tasks){ /////遍历输出/////
            t.show();
        }
    }
    void loadTasks() {
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
        sortTasks();
    }

    void saveTasks(){
        ofstream fout(filename, ios::trunc); // 覆盖式输出
        for (const auto& t : tasks) {
            fout << t.toString() << std::endl;
        }
    }
};

#endif