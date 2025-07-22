#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "Scheduler.h"
#include <queue>
#include <unordered_map>

class FCFSScheduler : public Scheduler {
public:
    FCFSScheduler();
    
    void addTask(const Task& task) override;
    Task* getNextTask() override;
    void taskCompleted(int task_id) override;
    void tick() override;
    std::string getName() const override { return "First-Come-First-Serve"; }
    void reset() override;
    
    bool hasTasks() const override;
    int getActiveTasksCount() const override;
    
private:
    std::queue<Task> ready_queue_;
    std::unordered_map<int, Task> active_tasks_;
    Task* current_task_;
};

#endif // FCFS_SCHEDULER_H
