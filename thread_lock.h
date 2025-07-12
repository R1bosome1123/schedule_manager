#ifndef THREAD_LOCK_H
#define THREAD_LOCK_H
#include <mutex>
#include <vector>
#include <atomic>
using namespace std;

template<typename Function,typename SharedType>
void call_with_lock(Function task_func, SharedType& shared,atomic<FLAG>& flag, mutex& mtx);




#endif