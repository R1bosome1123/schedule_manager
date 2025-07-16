#include "mainwindow.h"
#include <QMessageBox>
#include <QWidget>

// 测试current_user_name 和 tasks
extern std::string current_user_name;

MainWindow::MainWindow(const std::vector<task>& Tasks,client *networkclient,QWidget *parent)
    : QMainWindow(parent)
{
    networkClient = networkclient; // 初始化网络通信客户端
    this->current_user_tasks = Tasks; // 初始化当前用户任务列表
    this->resize(800, 600);
    this->setWindowTitle("日程管理系统");

    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // 日历控件
    calendar = new QCalendarWidget(centralWidget);
    calendar->setGeometry(30, 40, 421, 511);

    // 任务表格
    task_table = new QTableWidget(centralWidget);
    task_table->setGeometry(460, 260, 301, 271);

    // 创建右上角任务输入区布局
    QWidget* inputPanel = new QWidget(centralWidget);
    inputPanel->setGeometry(460, 40, 301, 171);

    QVBoxLayout* inputLayout = new QVBoxLayout(inputPanel);

    // 截止时间
    due_time = new QDateTimeEdit(inputPanel);
    due_time->setDateTime(QDateTime::currentDateTime()); // 初始化为当前的时刻
    qDebug() << "初始化时间：" << due_time->dateTime().toString();

    inputLayout->addWidget(due_time);

    // 提醒时间布局
    QHBoxLayout* remindLayout = new QHBoxLayout;
    remindLayout->addWidget(new QLabel("提前"));
    remind_before_day = new QSpinBox;
    remindLayout->addWidget(remind_before_day);
    remindLayout->addWidget(new QLabel("天"));
    remind_before_hour = new QSpinBox;
    remindLayout->addWidget(remind_before_hour);
    remindLayout->addWidget(new QLabel("小时"));
    remind_before_minute = new QSpinBox;
    remindLayout->addWidget(remind_before_minute);
    remindLayout->addWidget(new QLabel("分 提醒"));
    inputLayout->addLayout(remindLayout);

    // 输入任务名称
    Task_input = new QLineEdit;
    Task_input->setPlaceholderText("请输入日程");
    inputLayout->addWidget(Task_input);

    // 输入任务描述
    task_description_input = new QLineEdit;
    task_description_input->setPlaceholderText("请输入日程描述");
    inputLayout->addWidget(task_description_input);

    // 添加任务按钮
    add_task_button = new QPushButton("添加日程");
    inputLayout->addWidget(add_task_button);

    // 编辑/删除按钮区域
    QWidget* controlPanel = new QWidget(centralWidget);
    controlPanel->setGeometry(460, 200, 301, 64);

    QHBoxLayout* controlLayout = new QHBoxLayout(controlPanel);
    edit_task_button = new QPushButton("编辑日程");
    delete_task_button = new QPushButton("删除日程");
    controlLayout->addWidget(edit_task_button);
    controlLayout->addWidget(delete_task_button);

    // 状态栏和菜单栏
    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setGeometry(0, 0, 800, 27);
    this->setMenuBar(menuBar);

    QStatusBar* statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);

    Task_input->setPlaceholderText("输入日程");

    ///////////////////这里需要获得tasks,和后端对接
    manager = new task_manager(current_user_name, this->current_user_tasks);
    task_scanner = new scan(current_user_name); /////////////需要current_user_name传入

    // manager->load_tasks(tasks);

    connect(task_table, &QTableWidget::cellClicked, this, &MainWindow::on_task_table_cellClicked); //连接日历显示表格

    connect(calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::on_calendar_date_changed);     // 连接日历格点击

    connect(add_task_button, &QPushButton::clicked, this, &MainWindow::on_add_task_button_clicked);
    connect(delete_task_button, &QPushButton::clicked, this, &MainWindow::on_delete_task_button_clicked);
    connect(edit_task_button, &QPushButton::clicked, this, &MainWindow::on_edit_button_clicked);
    // 设置提醒扫描器定时器
    reminderTimer = new QTimer(this);
    connect(reminderTimer, &QTimer::timeout, this, &MainWindow::checkDueTasks);
    reminderTimer->start(10000); // 每10 second 扫描
}

MainWindow::~MainWindow()
{
    delete manager;
    delete reminderTimer;
    delete task_scanner;
}

void MainWindow::checkDueTasks()
{
    if (!task_scanner) return;

    // 包装一个 lock_access 适配器
    auto lock_access = [&](std::function<void(std::vector<task>&)> callback) {
        callback(current_user_tasks); // 直接操作 MainWindow 的 task 容器
    };

    // 获取当前日历选中的日期
    QDate selectedDate = calendar->selectedDate();

    // 找出这一天的任务
    std::vector<task> tasks_of_day;
    for (const auto& t : current_user_tasks) {
        QDate taskDate = QDateTime::fromSecsSinceEpoch(t.get_task_due_time()).date();
        if (taskDate == selectedDate) {
            tasks_of_day.push_back(t);
        }
    }

    task_scanner->scan_due_task(lock_access);  // 调用旧代码
    // refresh_task_table(tasks_of_day); // 更新表格显示
}

void MainWindow::on_calendar_date_changed() {
    QDate selectedDate = calendar->selectedDate();
    std::vector<task> tasks_of_day;

    for (const auto& t : current_user_tasks) {
        QDate taskDate = QDateTime::fromSecsSinceEpoch(t.get_task_due_time()).date();
        if (taskDate == selectedDate) {
            tasks_of_day.push_back(t);
        }
    }

    // 显示任务到 task_table
    refresh_task_table(tasks_of_day);
}

// 获取任务输入
void MainWindow::on_add_task_button_clicked()
{
    // 1. 获取任务名与描述
    QString name = Task_input->text();
    QString description = task_description_input->text(); // 你也可以增加描述输入框

    // 2. 获取截止时间
    QDateTime due = due_time->dateTime();
    QDate date = due.date();
    QTime time = due.time();

    // 3. 获取提醒时间间隔日期的槽函数
    int remind_day = remind_before_day->value();
    int remind_hour = remind_before_hour->value();
    int remind_minute = remind_before_minute->value();

    // 4. 构造 task
    task newTask(name.toStdString(), description.toStdString(),
                 date.year(), date.month(), date.day(),
                 time.hour(), time.minute(),
                 remind_day, remind_hour, remind_minute);

    // 5. 添加任务
    task_manager manager(current_user_name, current_user_tasks);
    manager.add_task(newTask, current_user_tasks);
    // manager.save_tasks(current_user_tasks);

    // 6. 清空界面控件
    Task_input->clear();
    task_description_input->clear();
    due_time->setDateTime(QDateTime::currentDateTime());
    remind_before_day->setValue(0);
    remind_before_hour->setValue(0);
    remind_before_minute->setValue(0);

    QMessageBox::information(this, "成功", "任务添加成功！");
    refresh_task_table();
}

// 显示当前用户日程
void MainWindow::refresh_task_table()
{
    // int currentRow = task_table->currentRow();

    task_table->setRowCount(0);  // 清空现有行
    task_table->setColumnCount(4); // 显示：任务名称、截止时间、提醒时间

    QStringList headers = {"任务名称", "任务描述", "截止时间", "提醒时间"};
    task_table->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < static_cast<int>(current_user_tasks.size()); ++i)
    {
        const task& t = current_user_tasks[i];
        task_table->insertRow(i);

        // 任务名称
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(t.get_task_name()));
        nameItem->setData(Qt::UserRole, t.get_task_id()); // 保存 task_id 到 UserRole
        task_table->setItem(i, 0, nameItem);

        // 任务描述
        QTableWidgetItem* descItem = new QTableWidgetItem(QString::fromStdString(t.get_task_description()));
        task_table->setItem(i, 1, descItem);
        // 截止时间
        QDateTime due = QDateTime::fromSecsSinceEpoch(t.get_task_due_time());
        QTableWidgetItem* dueItem = new QTableWidgetItem(due.toString("yyyy-MM-dd HH:mm:ss"));
        task_table->setItem(i, 2, dueItem);

        // 提醒时间
        QDateTime remind = QDateTime::fromSecsSinceEpoch(t.get_task_remind_time());
        QTableWidgetItem* remindItem = new QTableWidgetItem(remind.toString("yyyy-MM-dd HH:mm:ss"));
        task_table->setItem(i, 3, remindItem);


    }
    // 重新选中当前的行
    // if (currentRow >= 0 && currentRow < task_table->rowCount())
    // {
    //     task_table->setCurrentCell(currentRow, 0);
    // }

    task_table->resizeColumnsToContents();
    task_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止用户手动编辑表格
}

// 表格显示选中日期的日程
void MainWindow::refresh_task_table(const std::vector<task>& tasks)
{
    // int currentRow = task_table->currentRow();
    task_table->setRowCount(0);  // 清空现有表格
    task_table->setColumnCount(4);

    QStringList headers = {"任务名称", "任务描述", "截止时间", "提醒时间" };
    task_table->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < static_cast<int>(tasks.size()); ++i)
    {
        const task& t = tasks[i];

        task_table->insertRow(i);

        // 任务名称
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(t.get_task_name()));
        nameItem->setData(Qt::UserRole, t.get_task_id());
        task_table->setItem(i, 0, nameItem);

        // 任务描述
        QTableWidgetItem* descItem = new QTableWidgetItem(QString::fromStdString(t.get_task_description()));
        task_table->setItem(i, 1, descItem);
        // 截止时间
        QString due_str = QDateTime::fromSecsSinceEpoch(t.get_task_due_time()).toString("yyyy-MM-dd HH:mm:ss");
        QTableWidgetItem* dueItem = new QTableWidgetItem(due_str);
        task_table->setItem(i, 2, dueItem);

        // 提醒时间
        QString remind_str = QDateTime::fromSecsSinceEpoch(t.get_task_remind_time()).toString("yyyy-MM-dd HH:mm:ss");
        QTableWidgetItem* remindItem = new QTableWidgetItem(remind_str);
        task_table->setItem(i, 3, remindItem);
    }
    // 重新选中当前的行
    // if (currentRow >= 0 && currentRow < task_table->rowCount())
    // {
    //     task_table->setCurrentCell(currentRow, 0);
    // }
    task_table->resizeColumnsToContents();
    task_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止用户手动修改表格内容
}


void MainWindow::on_delete_task_button_clicked()
{
    int row = task_table->currentRow();  // 获取表格中选中行
    if (row < 0) {
        QMessageBox::warning(this, "错误", "请选择要删除的任务行！");
        return;
    }

    // 从表格第0列的 UserRole 取出 task_id
    QTableWidgetItem* item = task_table->item(row, 0);
    if (!item) {
        QMessageBox::warning(this, "错误", "任务信息获取失败！");
        return;
    }
    int task_id = item->data(Qt::UserRole).toInt();

    // 在 current_user_tasks 中查找该任务
    auto it = std::find_if(current_user_tasks.begin(), current_user_tasks.end(),
                           [task_id](const task& t) {
                               return t.get_task_id() == task_id;
                           });

    if (it != current_user_tasks.end()) {
        current_user_tasks.erase(it);  // 删除任务
        QMessageBox::information(this, "成功", "任务已删除！");
        refresh_task_table();          // 刷新表格
    } else {
        QMessageBox::warning(this, "错误", "未找到对应的任务！");
    }

}

void MainWindow::on_edit_button_clicked()
{
    int row = task_table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "错误", "请选择要修改的任务！");
        return;
    }

    // ✅ 提取选中任务的 task_id
    QTableWidgetItem* item = task_table->item(row, 0);  // 第0列是任务名列
    if (!item) {
        QMessageBox::warning(this, "错误", "无法获取任务信息！");
        return;
    }
    int task_id = item->data(Qt::UserRole).toInt();

    // ✅ 查找旧任务
    auto it = std::find_if(current_user_tasks.begin(), current_user_tasks.end(),
                           [task_id](const task& t) {
                               return t.get_task_id() == task_id;
                           });

    if (it == current_user_tasks.end()) {
        QMessageBox::warning(this, "错误", "未找到该任务！");
        return;
    }

    // 1. 获取新输入的数据
    QString name = Task_input->text();
    QString description = task_description_input->text();
    QDateTime due = due_time->dateTime();
    QDate date = due.date();
    QTime time = due.time();

    int y = date.year(), m = date.month(), d = date.day();
    int h = time.hour(), min = time.minute();

    int remind_day = remind_before_day->value();
    int remind_hour = remind_before_hour->value();
    int remind_minute = remind_before_minute->value();

    // 2. 构造新的任务，保留原有 task_id
    task new_task(name.toStdString(), description.toStdString(),
                  y, m, d, h, min,
                  remind_day, remind_hour, remind_minute, 0, task_id);

    // 3. 替换旧任务
    *it = new_task;

    QMessageBox::information(this, "成功", "任务已修改！");
    refresh_task_table();
}



// void MainWindow::on_task_table_cellClicked(int row, int column)
// {
//     if (row < 0 || row >= static_cast<int>(current_user_tasks.size())) return;

//     const task& t = current_user_tasks[row]; // 允许关闭事件

//     // 任务名
//     Task_input->setText(QString::fromStdString(t.get_task_name()));

//     // 任务描述
//     task_description_input->setText(QString::fromStdString(t.get_task_description()));

//     // 截止时间
//     time_t due_time = t.get_task_due_time();
//     this->due_time->setDateTime(QDateTime::fromSecsSinceEpoch(due_time)); // 这里如果注释，可以防止点击时清除在输入框中编辑好的任务

//     // 提醒时间
//     time_t remind_time = t.get_task_remind_time();

//     // 计算提前的总秒数（提醒时间与截止时间之差）
//     int delta_seconds = static_cast<int>(difftime(due_time, remind_time));
//     if (delta_seconds < 0) delta_seconds = 0; // 安全保障：防止负数

//     // 拆分为天、小时、分钟
//     int days = delta_seconds / 86400;
//     int hours = (delta_seconds % 86400) / 3600;
//     int minutes = (delta_seconds % 3600) / 60;

//     remind_before_day->setValue(days);
//     remind_before_hour->setValue(hours);
//     remind_before_minute->setValue(minutes);
// }


void MainWindow::on_task_table_cellClicked(int row, int column)
{
    if (row < 0) return;

    // 从表格的第0列取出 task_id
    QTableWidgetItem* item = task_table->item(row, 0);
    if (!item) return;

    int task_id = item->data(Qt::UserRole).toInt();

    // 在 current_user_tasks 中查找对应 task
    auto it = std::find_if(current_user_tasks.begin(), current_user_tasks.end(),
                           [task_id](const task& t) {
                               return t.get_task_id() == task_id;
                           });

    if (it == current_user_tasks.end()) return;

    const task& t = *it;

    // 更新输入框
    Task_input->setText(QString::fromStdString(t.get_task_name()));
    task_description_input->setText(QString::fromStdString(t.get_task_description()));

    // 截止时间
    time_t due_time = t.get_task_due_time();
    this->due_time->setDateTime(QDateTime::fromSecsSinceEpoch(due_time));

    // 提醒时间
    time_t remind_time = t.get_task_remind_time();
    int delta_seconds = static_cast<int>(difftime(due_time, remind_time));
    if (delta_seconds < 0) delta_seconds = 0;

    int days = delta_seconds / 86400;
    int hours = (delta_seconds % 86400) / 3600;
    int minutes = (delta_seconds % 3600) / 60;

    remind_before_day->setValue(days);
    remind_before_hour->setValue(hours);
    remind_before_minute->setValue(minutes);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 在关闭窗口时，可以选择保存任务或进行其他清理工作
    // manager->save_tasks(current_user_tasks); // 如果需要保存任务
    QString message;
    // this->networkClient.connectToServer(message); // 连接到服务器
    this->networkClient->sendAllTasks(current_user_tasks, message); // 发送任务到服务器
    this->networkClient->closeConnection(); // 关闭连接
    event->accept(); // 允许关闭事件
}
