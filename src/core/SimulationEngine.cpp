#include "SimulationEngine.h"
#include <iostream>
#include <algorithm>

SimulationEngine::SimulationEngine(std::unique_ptr<Scheduler> scheduler)
    : scheduler_(std::move(scheduler)), current_time_(0), total_energy_(0.0) {
    
    // Create default models if none provided
    energy_model_ = std::make_unique<EnergyModel>();
    thermal_model_ = std::make_unique<ThermalModel>();
}

SimulationStats SimulationEngine::runSimulation(const std::vector<Task>& tasks, int max_time) {
    reset();
    
    // Add all tasks to scheduler
    for (const auto& task : tasks) {
        scheduler_->addTask(task);
    }
    
    int idle_time = 0;
    const int max_idle_time = 1000; // Stop if idle for too long
    
    while (current_time_ < max_time && (scheduler_->hasTasks() || idle_time < max_idle_time)) {
        Task* current_task = scheduler_->getNextTask();
        
        if (current_task != nullptr) {
            idle_time = 0;
            executeTask(current_task);
            
            if (current_task->isCompleted()) {
                scheduler_->taskCompleted(current_task->getId());
                completed_tasks_.push_back(*current_task);
            }
        } else {
            idle_time++;
            // Idle power consumption
            updateModels(nullptr, 1);
        }
        
        scheduler_->tick();
        current_time_++;
    }
    
    // Prepare statistics
    SimulationStats stats;
    stats.total_tasks = completed_tasks_.size();
    stats.total_execution_time = current_time_;
    stats.total_energy_joules = total_energy_;
    stats.average_power_watts = (current_time_ > 0) ? total_energy_ / (current_time_ / 1000.0) : 0.0;
    stats.peak_temperature_celsius = thermal_model_->getPeakTemperature();
    stats.scheduler_name = scheduler_->getName();
    stats.completed_tasks = completed_tasks_;
    
    return stats;
}

void SimulationEngine::executeTask(Task* task) {
    if (task == nullptr) return;
    
    int execution_slice = 1; // Execute 1 tick at a time for fine-grained control
    task->execute(execution_slice);
    
    updateModels(task, execution_slice);
}

void SimulationEngine::updateModels(Task* task, int execution_time) {
    double frequency = scheduler_->getCurrentFrequency();
    double power = 0.0;
    
    if (task != nullptr) {
        // Active power consumption
        power = energy_model_->calculatePower(task->getType(), frequency);
        total_energy_ += energy_model_->calculateTaskEnergy(*task, frequency, execution_time);
    } else {
        // Idle power consumption (10% of active power at minimum frequency)
        power = energy_model_->calculatePower(TaskType::CPU_BOUND, 0.5) * 0.1;
        total_energy_ += power * (execution_time / 1000.0);
    }
    
    thermal_model_->updateTemperature(power);
}

void SimulationEngine::setEnergyModel(std::unique_ptr<EnergyModel> energy_model) {
    energy_model_ = std::move(energy_model);
}

void SimulationEngine::setThermalModel(std::unique_ptr<ThermalModel> thermal_model) {
    thermal_model_ = std::move(thermal_model);
}

double SimulationEngine::getCurrentPower() const {
    // This would need to track the last calculated power
    return 0.0; // Simplified for now
}

double SimulationEngine::getCurrentTemperature() const {
    return thermal_model_->getCurrentTemperature();
}

void SimulationEngine::reset() {
    scheduler_->reset();
    thermal_model_->reset();
    current_time_ = 0;
    total_energy_ = 0.0;
    completed_tasks_.clear();
}
