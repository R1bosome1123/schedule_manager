#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>
#include <string>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include "scan.h"
#include "task.h"
#include "FLAG.h"
#include "task_manager.h"
#include <QSoundEffect>
using namespace std;

void scan::scan_due_task(function<void(function<void(vector<task>& tasks)>)> lock_access)
{
    lock_access([&](vector<task>& tasks){this->examine_task(tasks);});
    this_thread::sleep_for(chrono::seconds(1)); 
}

void scan::examine_task(vector<task>& tasks) 
{
    //调试信息
    if (tasks.empty())
        return;
    auto now = time(nullptr);
    for (const auto& it : tasks)
    {
        if(it.get_task_remind_time() <= now && it.get_task_remind_time() >= now-10.1)
        {
            QSoundEffect effect;
            effect.setSource(QUrl::fromLocalFile("/usr/share/sounds/alsa/Noise.wav")); 
            effect.setVolume(0.7); 
            
            // 播放提示音
            effect.play();
            
            // 显示弹窗（音效会在弹窗期间播放）
            QMessageBox::information(
                nullptr, 
                "任务提醒", 
                QString("任务提醒：%1\n截止时间即将到来！").arg(QString::fromStdString(it.get_task_name()))
            );
            
            // 弹窗关闭后停止（如果音效未播完）
            effect.stop();
        }
    } 
}
