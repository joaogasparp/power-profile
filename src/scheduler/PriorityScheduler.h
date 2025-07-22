#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include "Scheduler.h"
#include <queue>
#include <unordered_map>

struct TaskPriorityComparator {
    bool operator()(const Task& a, const Task& b) const {
        // Higher priority value means higher priority
        return a.getPriority() < b.getPriority();
    }
};

class PriorityScheduler : public Scheduler {
public:
    explicit PriorityScheduler(bool preemptive = true);
    
    void addTask(const Task& task) override;
    Task* getNextTask() override;
    void taskCompleted(int task_id) override;
    void tick() override;
    std::string getName() const override { return "Priority-Based"; }
    void reset() override;
    
    bool hasTasks() const override;
    int getActiveTasksCount() const override;
    
private:
    std::priority_queue<Task, std::vector<Task>, TaskPriorityComparator> ready_queue_;
    std::unordered_map<int, Task> active_tasks_;
    Task* current_task_;
    bool preemptive_;
};

#endif // PRIORITY_SCHEDULER_H
