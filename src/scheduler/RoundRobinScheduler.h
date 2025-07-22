#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "Scheduler.h"
#include <queue>
#include <unordered_map>

class RoundRobinScheduler : public Scheduler {
public:
    explicit RoundRobinScheduler(int time_quantum = 10);
    
    void addTask(const Task& task) override;
    Task* getNextTask() override;
    void taskCompleted(int task_id) override;
    void tick() override;
    std::string getName() const override { return "Round-Robin"; }
    void reset() override;
    
    bool hasTasks() const override;
    int getActiveTasksCount() const override;
    
private:
    std::queue<Task> ready_queue_;
    std::unordered_map<int, Task> active_tasks_;
    Task* current_task_;
    int time_quantum_;
    int remaining_quantum_;
};

#endif // ROUND_ROBIN_SCHEDULER_H
