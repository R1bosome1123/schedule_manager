#ifndef THREAD_LOCK_H
#define THREAD_LOCK_H
#include <mutex>
#include <vector>
#include <atomic>
#include "FLAG.h"
#include <iostream>
#include <functional>

using namespace std;

template<typename Function,typename SharedType>
void call_with_lock(Function task_func, SharedType& shared,atomic<FLAG>& flag, mutex& mtx)
{
    while(flag) //SUCCESS_LOGGED_IN
    {
        auto lock_access = [&](function<void(SharedType&)> access_fn) {
            lock_guard<mutex> lock(mtx);    
            access_fn(shared);                        
        };
        
        task_func(lock_access);  
    }
}
#endif