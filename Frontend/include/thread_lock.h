#ifndef THREAD_LOCK_H
#define THREAD_LOCK_H
#include <mutex>
#include <vector>
#include <atomic>
#include "FLAG.h"
#include <iostream>
#include <functional>

using namespace std;

// This function template allows you to call a function with a lock on a shared resource.

template<typename Function,typename SharedType>
void call_with_lock(Function task_func, SharedType& shared,atomic<FLAG>& flag, mutex& mtx)
{


    auto lock_access = [&](function<void(SharedType&)> access_fn) {
        lock_guard<mutex> lock(mtx);    
        access_fn(shared);                        
    };


    while(flag) //SUCCESS_LOGGED_IN
    {
        task_func(lock_access);  
    }
}
#endif