#include "DVFSScheduler.h"

DVFSScheduler::DVFSScheduler() : current_task_(nullptr), current_frequency_(1.0) {
    // Available frequency levels (GHz)
    available_frequencies_ = {0.5, 0.8, 1.0, 1.2, 1.5, 2.0};
}

void DVFSScheduler::addTask(const Task& task) {
    ready_queue_.push(task);
    adjustFrequencyBasedOnLoad();
}

Task* DVFSScheduler::getNextTask() {
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
        
        // Adjust frequency based on task type and system load
        current_frequency_ = selectOptimalFrequency(current_task_);
        
        return current_task_;
    }
    
    current_task_ = nullptr;
    // Reduce frequency when idle
    current_frequency_ = available_frequencies_[0]; // Lowest frequency
    return nullptr;
}

void DVFSScheduler::taskCompleted(int task_id) {
    auto it = active_tasks_.find(task_id);
    if (it != active_tasks_.end()) {
        it->second.setEndTime(current_tick_);
        active_tasks_.erase(it);
    }
    current_task_ = nullptr;
    adjustFrequencyBasedOnLoad();
}

void DVFSScheduler::tick() {
    current_tick_++;
}

void DVFSScheduler::reset() {
    while (!ready_queue_.empty()) {
        ready_queue_.pop();
    }
    active_tasks_.clear();
    current_task_ = nullptr;
    current_tick_ = 0;
    current_frequency_ = 1.0;
}

bool DVFSScheduler::hasTasks() const {
    return !ready_queue_.empty() || !active_tasks_.empty();
}

int DVFSScheduler::getActiveTasksCount() const {
    return ready_queue_.size() + active_tasks_.size();
}

double DVFSScheduler::selectOptimalFrequency(const Task* task) const {
    if (task == nullptr) {
        return available_frequencies_[0]; // Minimum frequency
    }
    
    // Select frequency based on task type and urgency
    switch (task->getType()) {
        case TaskType::CPU_BOUND:
            // CPU-bound tasks benefit from higher frequency
            return available_frequencies_[4]; // 1.5 GHz
            
        case TaskType::IO_BOUND:
            // I/O-bound tasks don't need high frequency
            return available_frequencies_[1]; // 0.8 GHz
            
        case TaskType::MEMORY_HEAVY:
            // Memory-heavy tasks need moderate frequency
            return available_frequencies_[2]; // 1.0 GHz
            
        default:
            return available_frequencies_[2]; // Default 1.0 GHz
    }
}

void DVFSScheduler::adjustFrequencyBasedOnLoad() {
    int total_tasks = ready_queue_.size() + active_tasks_.size();
    
    if (total_tasks == 0) {
        current_frequency_ = available_frequencies_[0]; // Minimum
    } else if (total_tasks < 5) {
        current_frequency_ = available_frequencies_[2]; // Medium
    } else if (total_tasks < 10) {
        current_frequency_ = available_frequencies_[4]; // High
    } else {
        current_frequency_ = available_frequencies_[5]; // Maximum
    }
}
