#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>
#include <string>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include "task.h"  // 添加 task 类型声明
using namespace std;

class scan
{
private:
    string user_name;  
public:
    scan(string name) : user_name(name) {}
    void scan_due_task(function<void(function<void(vector<task>& tasks)>)> lock_access);
    void examine_task(vector<task>& tasks) ;
};





 