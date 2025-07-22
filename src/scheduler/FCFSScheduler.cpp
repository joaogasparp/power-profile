#include "FCFSScheduler.h"

FCFSScheduler::FCFSScheduler() : current_task_(nullptr) {
}

void FCFSScheduler::addTask(const Task& task) {
    ready_queue_.push(task);
}

Task* FCFSScheduler::getNextTask() {
    if (current_task_ != nullptr && !current_task_->isCompleted()) {
        return current_task_;
    }
    
    if (!ready_queue_.empty()) {
        Task task = ready_queue_.front();
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

void FCFSScheduler::taskCompleted(int task_id) {
    auto it = active_tasks_.find(task_id);
    if (it != active_tasks_.end()) {
        it->second.setEndTime(current_tick_);
        active_tasks_.erase(it);
    }
    current_task_ = nullptr;
}

void FCFSScheduler::tick() {
    current_tick_++;
}

void FCFSScheduler::reset() {
    while (!ready_queue_.empty()) {
        ready_queue_.pop();
    }
    active_tasks_.clear();
    current_task_ = nullptr;
    current_tick_ = 0;
}

bool FCFSScheduler::hasTasks() const {
    return !ready_queue_.empty() || !active_tasks_.empty();
}

int FCFSScheduler::getActiveTasksCount() const {
    return ready_queue_.size() + active_tasks_.size();
}
