#ifndef TASK_H
#define TASK_H

#include <string>

enum class TaskType {
    CPU_BOUND,
    IO_BOUND,
    MEMORY_HEAVY
};

class Task {
public:
    Task() : id_(0), duration_(0), remaining_time_(0), type_(TaskType::CPU_BOUND), priority_(0), start_time_(-1), end_time_(-1) {}
    Task(int id, int duration, TaskType type, int priority = 0);
    
    // Getters
    int getId() const { return id_; }
    int getDuration() const { return duration_; }
    int getRemainingTime() const { return remaining_time_; }
    TaskType getType() const { return type_; }
    int getPriority() const { return priority_; }
    bool isCompleted() const { return remaining_time_ <= 0; }
    
    // Task execution
    void execute(int time_slice);
    void reset();
    
    // Statistics
    int getStartTime() const { return start_time_; }
    int getEndTime() const { return end_time_; }
    void setStartTime(int time) { start_time_ = time; }
    void setEndTime(int time) { end_time_ = time; }
    
    std::string getTypeString() const;
    
private:
    int id_;
    int duration_;
    int remaining_time_;
    TaskType type_;
    int priority_;
    int start_time_;
    int end_time_;
};

#endif // TASK_H
