#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "tasks/Task.h"
#include <vector>
#include <queue>
#include <string>

class Scheduler {
public:
    virtual ~Scheduler() = default;
    
    // Pure virtual methods that must be implemented by derived classes
    virtual void addTask(const Task& task) = 0;
    virtual Task* getNextTask() = 0;
    virtual void taskCompleted(int task_id) = 0;
    virtual void tick() = 0;
    virtual std::string getName() const = 0;
    virtual void reset() = 0;
    
    // Common functionality
    virtual bool hasTasks() const = 0;
    virtual int getActiveTasksCount() const = 0;
    virtual double getCurrentFrequency() const { return 1.0; } // Default 1.0 GHz
    
protected:
    int current_tick_;
    
public:
    Scheduler() : current_tick_(0) {}
    int getCurrentTick() const { return current_tick_; }
};

#endif // SCHEDULER_H
