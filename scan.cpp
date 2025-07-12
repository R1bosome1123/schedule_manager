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
#include "schedule.h"
using namespace std;

void scan::scan_due_task(function<void(vector<task>&)> lock_access)
{
        lock_access(examine_task);
        this_thread::sleep_for(chrono::seconds(1)); 
}
void scan::examine_task(vector<task>& tasks) 
{
    if (tasks.empty())
        return;
    auto now = time(nullptr);
    auto it = tasks.begin();
    if(it->time == now)
    {
        it->show();
        tasks.erase(it);
    }
}


 