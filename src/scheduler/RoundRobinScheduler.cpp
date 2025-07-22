#include "RoundRobinScheduler.h"

RoundRobinScheduler::RoundRobinScheduler(int time_quantum) 
    : time_quantum_(time_quantum), remaining_quantum_(time_quantum), current_task_(nullptr) {
}

void RoundRobinScheduler::addTask(const Task& task) {
    ready_queue_.push(task);
}

Task* RoundRobinScheduler::getNextTask() {
    if (current_task_ != nullptr && !current_task_->isCompleted() && remaining_quantum_ > 0) {
        return current_task_;
    }
    
    // Time quantum expired or task completed, get next task
    if (current_task_ != nullptr && !current_task_->isCompleted()) {
        // Put back in ready queue if not completed
        ready_queue_.push(*current_task_);
        current_task_ = nullptr;
    }
    
    if (!ready_queue_.empty()) {
        Task task = ready_queue_.front();
        ready_queue_.pop();
        
        active_tasks_[task.getId()] = task;
        current_task_ = &active_tasks_[task.getId()];
        remaining_quantum_ = time_quantum_;
        
        if (current_task_->getStartTime() == -1) {
            current_task_->setStartTime(current_tick_);
        }
        
        return current_task_;
    }
    
    current_task_ = nullptr;
    return nullptr;
}

void RoundRobinScheduler::taskCompleted(int task_id) {
    auto it = active_tasks_.find(task_id);
    if (it != active_tasks_.end()) {
        it->second.setEndTime(current_tick_);
        active_tasks_.erase(it);
    }
    current_task_ = nullptr;
}

void RoundRobinScheduler::tick() {
    current_tick_++;
    if (remaining_quantum_ > 0) {
        remaining_quantum_--;
    }
}

void RoundRobinScheduler::reset() {
    while (!ready_queue_.empty()) {
        ready_queue_.pop();
    }
    active_tasks_.clear();
    current_task_ = nullptr;
    current_tick_ = 0;
    remaining_quantum_ = time_quantum_;
}

bool RoundRobinScheduler::hasTasks() const {
    return !ready_queue_.empty() || !active_tasks_.empty();
}

int RoundRobinScheduler::getActiveTasksCount() const {
    return ready_queue_.size() + active_tasks_.size();
}
