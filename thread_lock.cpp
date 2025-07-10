#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>
using namespace std;

template<typename Function, typename SharedType>
void call_with_lock(Function task_func, SharedType& shared, mutex& mtx,atomic<FLAG>& flag) 
{
    auto lock_access = [&](function<void(SharedType&)> access_fn) {
        lock_guard<mutex> lock(mtx);    
        access_fn(shared);                        
    };
    while(flag != WAIT_LOGGED_IN)
    {
        task_func(lock_access);  
    }

}

void He_Function(function<void(vector<task>&)> lock_access) 
{
    task current_task;
    task_info_guide();
    input_change_task(&current_task);
    auto add_task_known=[](vector<task>&tasks){return add_task(current_task, tasks);};
    lock_access(add_task_known);
    cout<< "Task added successfully." << endl;
    lock_access(sort_tasks);
}

void add_task(vector<task>& tasks, const task& current_task) 
{
    ///////
}
void sort_tasks(vector<task>& tasks) 
{
    /////////////////// 
}