#include<iostream>
#include <string>
#include <unistd.h> 
#include <atomic>
#include "info_prompt.h"
using namespace std;

void quit_program()
{
    cout<<"Exiting the program. Goodbye!"<<endl;
    sleep(1); // Sleep for 1 second before exiting
}


void get_user_credentials(string &username, string &password)
{
    cout<<"Username: ";
    cin>>username;
    if(username == "quit") {
        return; // Exit if user types 'quit'
    }
    // Prompt for password
    cout<<"Please enter your password."<<endl;
    cout<<"Note: Passwords are case-sensitive."<<endl;
    cout<<"Password: ";
    cin>>password;
}

void welcome_message()
{
    cout<<"Welcome to the Task Manager!"<<endl;
    cout<<"Please enter your credentials to continue."<<endl;
    cout<<"Type 'quit' to exit the program."<<endl;
}