#include "EnergyModel.h"
#include <cmath>

EnergyModel::EnergyModel(double base_capacitance, double base_voltage)
    : base_capacitance_(base_capacitance), base_voltage_(base_voltage),
      cpu_bound_multiplier_(1.2), io_bound_multiplier_(0.6), memory_multiplier_(0.9) {
}

double EnergyModel::calculateTaskEnergy(const Task& task, double frequency_ghz, int execution_time) {
    double power = calculatePower(task.getType(), frequency_ghz);
    // Energy = Power * Time (assuming execution_time is in simulation ticks, convert to seconds)
    double time_seconds = execution_time / 1000.0; // 1 tick = 1ms
    return power * time_seconds; // Joules
}

double EnergyModel::calculatePower(TaskType task_type, double frequency_ghz) {
    double voltage = getVoltageForFrequency(frequency_ghz);
    double task_multiplier = getTaskTypeMultiplier(task_type);
    
    // Power = C * V² * f * activity_factor
    // Simplified model: P = C * V² * f (Watts)
    double power = base_capacitance_ * voltage * voltage * frequency_ghz * 1e9 * task_multiplier;
    
    return power;
}

double EnergyModel::getVoltageForFrequency(double frequency_ghz) const {
    // Simplified voltage scaling: V = V_base * (f/f_base)^0.8
    // Assuming base frequency is 1.0 GHz
    double frequency_ratio = frequency_ghz / 1.0;
    return base_voltage_ * std::pow(frequency_ratio, 0.8);
}

void EnergyModel::setTaskTypeMultipliers(double cpu_bound_mult, double io_bound_mult, double memory_mult) {
    cpu_bound_multiplier_ = cpu_bound_mult;
    io_bound_multiplier_ = io_bound_mult;
    memory_multiplier_ = memory_mult;
}

double EnergyModel::getTaskTypeMultiplier(TaskType type) const {
    switch (type) {
        case TaskType::CPU_BOUND:
            return cpu_bound_multiplier_;
        case TaskType::IO_BOUND:
            return io_bound_multiplier_;
        case TaskType::MEMORY_HEAVY:
            return memory_multiplier_;
        default:
            return 1.0;
    }
}
