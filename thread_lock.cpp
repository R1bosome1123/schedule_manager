#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>

#include "thread"
#include "flag.h"
using namespace std;

template<typename Function, typename SharedType>
void call_with_lock(Function task_func, SharedType& shared,atomic<FLAG>& flag,mutex& mtx) 
{
    auto lock_access = [&](function<void(SharedType&)> access_fn) {
        lock_guard<mutex> lock(mtx);    
        access_fn(shared);                        
    };
    while(SUCEESS_LOGGED_IN)
    {
        task_func(lock_access);  
    }

}

