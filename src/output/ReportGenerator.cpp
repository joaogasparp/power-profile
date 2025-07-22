#include "ReportGenerator.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

void ReportGenerator::printConsoleReport(const SimulationStats& stats) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "         SIMULATION RESULTS" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "Scheduler: " << stats.scheduler_name << std::endl;
    std::cout << "Total Tasks: " << stats.total_tasks << std::endl;
    std::cout << "Completed Tasks: " << stats.completed_tasks.size() << std::endl;
    std::cout << "Total Execution Time: " << formatDuration(stats.total_execution_time) << std::endl;
    std::cout << "Total Energy Consumption: " << formatEnergy(stats.total_energy_joules) << std::endl;
    std::cout << "Average Power: " << formatPower(stats.average_power_watts) << std::endl;
    std::cout << "Peak Temperature: " << formatTemperature(stats.peak_temperature_celsius) << std::endl;
    
    if (!stats.completed_tasks.empty()) {
        // Calculate average turnaround time
        double total_turnaround = 0.0;
        int completed_count = 0;
        
        for (const auto& task : stats.completed_tasks) {
            if (task.getStartTime() >= 0 && task.getEndTime() >= 0) {
                total_turnaround += (task.getEndTime() - task.getStartTime());
                completed_count++;
            }
        }
        
        if (completed_count > 0) {
            double avg_turnaround = total_turnaround / completed_count;
            std::cout << "Average Turnaround Time: " << formatDuration(static_cast<int>(avg_turnaround)) << std::endl;
        }
    }
    
    std::cout << std::string(50, '=') << std::endl;
}

bool ReportGenerator::generateCSVReport(const SimulationStats& stats, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "scheduler,total_tasks,completed_tasks,execution_time_ticks,total_energy_J,average_power_W,peak_temp_C\n";
    file << stats.scheduler_name << ","
         << stats.total_tasks << ","
         << stats.completed_tasks.size() << ","
         << stats.total_execution_time << ","
         << std::fixed << std::setprecision(6) << stats.total_energy_joules << ","
         << std::fixed << std::setprecision(3) << stats.average_power_watts << ","
         << std::fixed << std::setprecision(2) << stats.peak_temperature_celsius << "\n";
    
    file.close();
    return true;
}

bool ReportGenerator::generateTaskCSV(const SimulationStats& stats, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "task_id,start_time,end_time,duration,type,scheduler,turnaround_time\n";
    
    for (const auto& task : stats.completed_tasks) {
        int turnaround_time = (task.getStartTime() >= 0 && task.getEndTime() >= 0) ?
                             task.getEndTime() - task.getStartTime() : -1;
        
        file << task.getId() << ","
             << task.getStartTime() << ","
             << task.getEndTime() << ","
             << task.getDuration() << ","
             << task.getTypeString() << ","
             << stats.scheduler_name << ","
             << turnaround_time << "\n";
    }
    
    file.close();
    return true;
}

void ReportGenerator::compareResults(const std::vector<SimulationStats>& results) {
    if (results.empty()) return;
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "                    SCHEDULER COMPARISON" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    std::cout << std::left << std::setw(15) << "Scheduler"
              << std::setw(12) << "Tasks"
              << std::setw(12) << "Time (ticks)"
              << std::setw(15) << "Energy (J)"
              << std::setw(12) << "Avg Power (W)"
              << std::setw(12) << "Peak Temp (°C)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& stat : results) {
        std::cout << std::left << std::setw(15) << stat.scheduler_name
                  << std::setw(12) << stat.completed_tasks.size()
                  << std::setw(12) << stat.total_execution_time
                  << std::setw(15) << std::fixed << std::setprecision(6) << stat.total_energy_joules
                  << std::setw(12) << std::fixed << std::setprecision(3) << stat.average_power_watts
                  << std::setw(12) << std::fixed << std::setprecision(2) << stat.peak_temperature_celsius << std::endl;
    }
    std::cout << std::string(80, '=') << std::endl;
}

std::string ReportGenerator::formatDuration(int ticks) {
    std::ostringstream oss;
    oss << ticks << " ticks (" << std::fixed << std::setprecision(2) << (ticks / 1000.0) << "s)";
    return oss.str();
}

std::string ReportGenerator::formatEnergy(double joules) {
    std::ostringstream oss;
    if (joules < 1e-3) {
        oss << std::fixed << std::setprecision(3) << (joules * 1e6) << " µJ";
    } else if (joules < 1.0) {
        oss << std::fixed << std::setprecision(3) << (joules * 1e3) << " mJ";
    } else {
        oss << std::fixed << std::setprecision(3) << joules << " J";
    }
    return oss.str();
}

std::string ReportGenerator::formatPower(double watts) {
    std::ostringstream oss;
    if (watts < 1e-3) {
        oss << std::fixed << std::setprecision(3) << (watts * 1e6) << " µW";
    } else if (watts < 1.0) {
        oss << std::fixed << std::setprecision(3) << (watts * 1e3) << " mW";
    } else {
        oss << std::fixed << std::setprecision(3) << watts << " W";
    }
    return oss.str();
}

std::string ReportGenerator::formatTemperature(double celsius) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << celsius << " °C";
    return oss.str();
}
