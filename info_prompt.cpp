#include<iostream>
#include <string>
#include <unistd.h> 
#include <atomic>
#include "info_prompt.h"
using namespace std;

void info_prompt::quit_program()
{
    cout<<"Exiting the program. Goodbye!"<<endl;
    sleep(1); // Sleep for 1 second before exiting
}


void info_prompt::log_out(string &user_name , vector<task> &tasks)
{
    cout<<"You have been logged out."<<endl;
    current_user_name.clear();
    tasks.clear(); 
}


void info_prompt::welcome_message()
{
    cout<<"Welcome to the Task Manager!"<<endl;
    cout<<"Please enter your credentials to continue."<<endl;
    cout<<"Type 'quit' to exit the program."<<endl;
}