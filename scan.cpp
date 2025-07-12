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
#include "scan.h"
#include "task.h"
#include "FLAG.h"
#include "task_manager.h"
using namespace std;

void scan::scan_due_task(function<void(function<void(vector<task>& tasks)>)> lock_access)
{
        lock_access([&](vector<task>& tasks){this->examine_task(tasks);});
        this_thread::sleep_for(chrono::seconds(1)); 
}
void scan::examine_task(vector<task>& tasks) 
{
    if (tasks.empty())
        return;
    auto now = time(nullptr);
    auto it = tasks.begin();
    if(it->startTime == now)
    {
        it->show();
        tasks.erase(it);
    }
}


 