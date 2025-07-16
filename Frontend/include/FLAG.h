#ifndef FLAG_H
#define FLAG_H
#include <atomic>

enum FLAG
{
    WAIT_LOGGED_IN = 0,
    SUCCESS_LOGGED_IN = 1,
};
extern std::atomic<FLAG> flag; 
#endif // FLAG_H