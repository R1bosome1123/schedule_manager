void He_Function(function<void(vector<task>&)> lock_access) 
{
    auto read_from_files_known = [](const string& user_name) {
        read_from_file(user_name, tasks);
    };
    task current_task;
    task_info_guide();
    input_change_task(&current_task);
    auto add_task_known=[](vector<task>&tasks){add_task(current_task, tasks);};
    lock_access(add_task_known);
    cout<< "Task added successfully." << endl;
    lock_access(sort_tasks);
}

void read_from_file(const string& user_name, vector<task>& tasks) 
{
    string file_path = Hash(user_name) + ".txt";
    ifstream file(file_path);
    /////////////
}

void add_task(vector<task>& tasks, const task& current_task) 
{
    ///////
}
void sort_tasks(vector<task>& tasks) 
{
    /////////////////// 
}