#ifndef TASK_GENERATOR_H
#define TASK_GENERATOR_H

#include "Task.h"
#include <vector>
#include <random>

class TaskGenerator {
public:
    TaskGenerator(unsigned int seed = 0);
    
    std::vector<Task> generateTasks(int num_tasks, 
                                  int min_duration = 50, 
                                  int max_duration = 500);
    
    void setTaskTypeDistribution(double cpu_bound_prob, 
                               double io_bound_prob, 
                               double memory_heavy_prob);
    
    void setPriorityRange(int min_priority, int max_priority);
    
private:
    std::mt19937 rng_;
    double cpu_bound_prob_;
    double io_bound_prob_;
    double memory_heavy_prob_;
    int min_priority_;
    int max_priority_;
    
    TaskType selectRandomTaskType();
    int generateRandomPriority();
};

#endif // TASK_GENERATOR_H
