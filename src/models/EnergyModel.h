#ifndef ENERGY_MODEL_H
#define ENERGY_MODEL_H

#include "tasks/Task.h"

class EnergyModel {
public:
    EnergyModel(double base_capacitance = 1e-9, double base_voltage = 1.2);
    
    // Calculate energy consumption for a task execution
    double calculateTaskEnergy(const Task& task, double frequency_ghz, int execution_time);
    
    // Calculate instantaneous power consumption
    double calculatePower(TaskType task_type, double frequency_ghz);
    
    // Get voltage for a given frequency (simplified model)
    double getVoltageForFrequency(double frequency_ghz) const;
    
    // Task type specific power multipliers
    void setTaskTypeMultipliers(double cpu_bound_mult, double io_bound_mult, double memory_mult);
    
private:
    double base_capacitance_;  // Effective capacitance (F)
    double base_voltage_;      // Base voltage (V)
    double cpu_bound_multiplier_;
    double io_bound_multiplier_;
    double memory_multiplier_;
    
    double getTaskTypeMultiplier(TaskType type) const;
};

#endif // ENERGY_MODEL_H
