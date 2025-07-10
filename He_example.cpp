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