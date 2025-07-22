#include "PriorityScheduler.h"

PriorityScheduler::PriorityScheduler(bool preemptive) 
    : preemptive_(preemptive), current_task_(nullptr) {
}

void PriorityScheduler::addTask(const Task& task) {
    ready_queue_.push(task);
    
    // If preemptive and we have a current task, check if new task has higher priority
    if (preemptive_ && current_task_ != nullptr) {
        if (task.getPriority() > current_task_->getPriority()) {
            // Preempt current task
            ready_queue_.push(*current_task_);
            current_task_ = nullptr;
        }
    }
}

Task* PriorityScheduler::getNextTask() {
    if (current_task_ != nullptr && !current_task_->isCompleted()) {
        return current_task_;
    }
    
    if (!ready_queue_.empty()) {
        Task task = ready_queue_.top();
        ready_queue_.pop();
        
        active_tasks_[task.getId()] = task;
        current_task_ = &active_tasks_[task.getId()];
        
        if (current_task_->getStartTime() == -1) {
            current_task_->setStartTime(current_tick_);
        }
        
        return current_task_;
    }
    
    current_task_ = nullptr;
    return nullptr;
}

void PriorityScheduler::taskCompleted(int task_id) {
    auto it = active_tasks_.find(task_id);
    if (it != active_tasks_.end()) {
        it->second.setEndTime(current_tick_);
        active_tasks_.erase(it);
    }
    current_task_ = nullptr;
}

void PriorityScheduler::tick() {
    current_tick_++;
}

void PriorityScheduler::reset() {
    // Clear priority queue
    while (!ready_queue_.empty()) {
        ready_queue_.pop();
    }
    active_tasks_.clear();
    current_task_ = nullptr;
    current_tick_ = 0;
}

bool PriorityScheduler::hasTasks() const {
    return !ready_queue_.empty() || !active_tasks_.empty();
}

int PriorityScheduler::getActiveTasksCount() const {
    return ready_queue_.size() + active_tasks_.size();
}
