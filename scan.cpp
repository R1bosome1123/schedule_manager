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
using namespace std;

void Chen_Function(function<void(vector<task>&)> lock_access) 
{
   while(true)
   {
        
        lock_access(examine_task);
        this_thread::sleep_for(chrono::seconds(1)); 
   }
}
void examine_task(vector<task>& tasks) 
{
    if (tasks.empty())
        return;
    auto now = time(nullptr);
    auto it = tasks.begin();
    if(it->time == now)
    {
        cout<<it.shedule<<endl;
        tasks.erase(it);
    }
}


 