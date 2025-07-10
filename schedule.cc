#include "schedule.h"
#include "useraccount.h"
#include <vector>
#include <iostream>
using namespace std;

// 简单 hash 函数（在 schedule.h 里也有，建议移过去）
string hash_username(const string &username)
{
    hash<string> hasher;
    return to_string(hasher(username));
}

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

int main(int argc, char *argv[])
{
    usermanager um;
    if (um.user_login() != 0)
        return 1; // 登录失败 直接退出

    string username = um.get_currentuser();
    TaskManager tm(username);
    tm.loadTasks();

    static int id = 1; // 简单的id自增,可持久到文件优化

    if (argc == 1 || (argc == 2 && string(argv[1]) == "run"))
    {
        cout << "进入日程管理 shell:\n";
        while (true)
        {
            cout << "\n请选择: 1.添加任务 2.查看任务 3.保存退出\n";
            int op;
            if (!(cin >> op))
                break;

            if (op == 1)//获取
            {
                string name;
                int y, m, d, h, min, remind;
                cout << "任务名：";
                cin.ignore();
                getline(cin, name);
                cout << "请输入时间 格式: 年 月 日 时 分：";
                cin >> y >> m >> d >> h >> min;
                cout << "提前提醒 秒：";
                cin >> remind;
                task t(name, y, m, d, h, min, remind, id++);
                tm.addTask(t);
            }
            else if (op == 2)
            {
                tm.showTasks();
            }
            else if (op == 3)
            {
                tm.saveTasks();
                cout << "已保存，退出。\n";
                break;
            }
            else
            {
                cout << "无效选项。\n";
            }
        }
    }
    else if (argc >= 2 && string(argv[1]) == "addtask")
    {
        if (argc != 9)
        {
            cerr << "错误: 用法 addtask taskname year month day hour minute remind_before\n";
            return 1;
        }
        string taskname = argv[2];
        int y = stoi(argv[3]), mo = stoi(argv[4]), d = stoi(argv[5]),
            h = stoi(argv[6]), mi = stoi(argv[7]), remind = stoi(argv[8]);
        task t(taskname, y, mo, d, h, mi, remind, id++);
        tm.addTask(t);
        tm.saveTasks();
    }
    else if (argc >= 2 && string(argv[1]) == "showtask")
    {
        tm.showTasks();
    }
    else
    {
        show_help();
    }

    return 0;
}