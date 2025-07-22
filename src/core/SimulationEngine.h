#ifndef SIMULATION_ENGINE_H
#define SIMULATION_ENGINE_H

#include "scheduler/Scheduler.h"
#include "models/EnergyModel.h"
#include "models/ThermalModel.h"
#include "tasks/Task.h"
#include <vector>
#include <memory>

struct SimulationStats {
    int total_tasks;
    int total_execution_time;
    double total_energy_joules;
    double average_power_watts;
    double peak_temperature_celsius;
    std::string scheduler_name;
    std::vector<Task> completed_tasks;
};

class SimulationEngine {
public:
    SimulationEngine(std::unique_ptr<Scheduler> scheduler);
    
    // Run simulation with given tasks
    SimulationStats runSimulation(const std::vector<Task>& tasks, int max_time = 50000);
    
    // Set models
    void setEnergyModel(std::unique_ptr<EnergyModel> energy_model);
    void setThermalModel(std::unique_ptr<ThermalModel> thermal_model);
    
    // Get current state
    double getCurrentPower() const;
    double getCurrentTemperature() const;
    
    // Reset simulation state
    void reset();
    
private:
    std::unique_ptr<Scheduler> scheduler_;
    std::unique_ptr<EnergyModel> energy_model_;
    std::unique_ptr<ThermalModel> thermal_model_;
    
    int current_time_;
    double total_energy_;
    std::vector<Task> completed_tasks_;
    
    void executeTask(Task* task);
    void updateModels(Task* task, int execution_time);
};

#endif // SIMULATION_ENGINE_H
