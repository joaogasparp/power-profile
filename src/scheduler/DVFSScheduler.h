#ifndef DVFS_SCHEDULER_H
#define DVFS_SCHEDULER_H

#include "Scheduler.h"
#include <queue>
#include <unordered_map>
#include <vector>

class DVFSScheduler : public Scheduler {
public:
    DVFSScheduler();
    
    void addTask(const Task& task) override;
    Task* getNextTask() override;
    void taskCompleted(int task_id) override;
    void tick() override;
    std::string getName() const override { return "DVFS-Aware"; }
    void reset() override;
    
    bool hasTasks() const override;
    int getActiveTasksCount() const override;
    double getCurrentFrequency() const override { return current_frequency_; }
    
private:
    std::queue<Task> ready_queue_;
    std::unordered_map<int, Task> active_tasks_;
    Task* current_task_;
    
    double current_frequency_;
    std::vector<double> available_frequencies_;
    
    double selectOptimalFrequency(const Task* task) const;
    void adjustFrequencyBasedOnLoad();
};

#endif // DVFS_SCHEDULER_H
