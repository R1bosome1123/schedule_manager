#include <QMainWindow>
#include <QCalendarWidget>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStatusBar>
#include <QMenuBar>
#include "task_manager.h"
#include "scan.h"
#include <vector>
#include <QTimer>
#include <QCloseEvent>

#include "client.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const std::vector<task>& Tasks,client *networkclient,QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event) override;

private:
    QCalendarWidget* calendar;
    QDateTimeEdit* due_time;
    QSpinBox* remind_before_day;
    QSpinBox* remind_before_hour;
    QSpinBox* remind_before_minute;
    QLineEdit* Task_input;
    QLineEdit* task_description_input;
    QPushButton* add_task_button;
    QPushButton* edit_task_button;
    QPushButton* delete_task_button;
    QTableWidget* task_table;

    task_manager *manager;
    std::vector<task> current_user_tasks;
    QTimer *reminderTimer;
    scan *task_scanner = nullptr;
    client *networkClient; // 网络通信客户端

    void refresh_task_table(const std::vector<task>& task_list);
    void refresh_task_table();

private slots:
    void on_add_task_button_clicked(); // 添加
    void on_delete_task_button_clicked();   // 删除
    void on_edit_button_clicked();          // 编辑（修改任务）
    void on_task_table_cellClicked(int row, int column); // 点击表格触发填入输入框
    void checkDueTasks();  // Qt 定时器的槽函数
    void on_calendar_date_changed(); // 点击日期的槽函数
};
