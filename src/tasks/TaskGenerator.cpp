#include "TaskGenerator.h"
#include <algorithm>

TaskGenerator::TaskGenerator(unsigned int seed) 
    : rng_(seed), cpu_bound_prob_(0.4), io_bound_prob_(0.3), 
      memory_heavy_prob_(0.3), min_priority_(1), max_priority_(10) {
}

std::vector<Task> TaskGenerator::generateTasks(int num_tasks, 
                                             int min_duration, 
                                             int max_duration) {
    std::vector<Task> tasks;
    tasks.reserve(num_tasks);
    
    std::uniform_int_distribution<> duration_dist(min_duration, max_duration);
    
    for (int i = 0; i < num_tasks; ++i) {
        int duration = duration_dist(rng_);
        TaskType type = selectRandomTaskType();
        int priority = generateRandomPriority();
        
        tasks.emplace_back(i, duration, type, priority);
    }
    
    return tasks;
}

void TaskGenerator::setTaskTypeDistribution(double cpu_bound_prob, 
                                          double io_bound_prob, 
                                          double memory_heavy_prob) {
    // Normalize probabilities
    double total = cpu_bound_prob + io_bound_prob + memory_heavy_prob;
    cpu_bound_prob_ = cpu_bound_prob / total;
    io_bound_prob_ = io_bound_prob / total;
    memory_heavy_prob_ = memory_heavy_prob / total;
}

void TaskGenerator::setPriorityRange(int min_priority, int max_priority) {
    min_priority_ = min_priority;
    max_priority_ = max_priority;
}

TaskType TaskGenerator::selectRandomTaskType() {
    std::uniform_real_distribution<> dist(0.0, 1.0);
    double rand_val = dist(rng_);
    
    if (rand_val < cpu_bound_prob_) {
        return TaskType::CPU_BOUND;
    } else if (rand_val < cpu_bound_prob_ + io_bound_prob_) {
        return TaskType::IO_BOUND;
    } else {
        return TaskType::MEMORY_HEAVY;
    }
}

int TaskGenerator::generateRandomPriority() {
    std::uniform_int_distribution<> priority_dist(min_priority_, max_priority_);
    return priority_dist(rng_);
}
