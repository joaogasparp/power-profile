#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <iomanip>
#include <fstream>

#include "core/SimulationEngine.h"
#include "scheduler/RoundRobinScheduler.h"
#include "scheduler/PriorityScheduler.h"
#include "scheduler/FCFSScheduler.h"
#include "scheduler/DVFSScheduler.h"
#include "tasks/TaskGenerator.h"
#include "output/ReportGenerator.h"

struct SimulationConfig {
    std::string scheduler_type = "RR";
    int num_tasks = 50;
    int simulation_duration = 10000;
    std::string output_file = "";
    bool compare_schedulers = false;
    unsigned int seed = 42;
    int min_task_duration = 50;
    int max_task_duration = 500;
    bool verbose = false;
};

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --scheduler TYPE    Scheduler type: RR (Round-Robin), Priority, FCFS, DVFS\n";
    std::cout << "  --tasks NUM         Number of tasks to simulate (default: 50)\n";
    std::cout << "  --duration TIME     Maximum simulation time in ticks (default: 10000)\n";
    std::cout << "  --output FILE       Output CSV file for results\n";
    std::cout << "  --compare           Compare all available schedulers\n";
    std::cout << "  --seed NUM          Random seed for task generation (default: 42)\n";
    std::cout << "  --min-duration NUM  Minimum task duration (default: 50)\n";
    std::cout << "  --max-duration NUM  Maximum task duration (default: 500)\n";
    std::cout << "  --verbose           Enable verbose output\n";
    std::cout << "  --help              Show this help message\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << program_name << " --scheduler RR --tasks 100 --duration 15000\n";
    std::cout << "  " << program_name << " --compare --tasks 75 --output comparison.csv\n";
}

SimulationConfig parseArguments(int argc, char* argv[]) {
    SimulationConfig config;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--scheduler") == 0 && i + 1 < argc) {
            config.scheduler_type = argv[++i];
        } else if (strcmp(argv[i], "--tasks") == 0 && i + 1 < argc) {
            config.num_tasks = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "--duration") == 0 && i + 1 < argc) {
            config.simulation_duration = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            config.output_file = argv[++i];
        } else if (strcmp(argv[i], "--compare") == 0) {
            config.compare_schedulers = true;
        } else if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc) {
            config.seed = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "--min-duration") == 0 && i + 1 < argc) {
            config.min_task_duration = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "--max-duration") == 0 && i + 1 < argc) {
            config.max_task_duration = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "--verbose") == 0) {
            config.verbose = true;
        } else if (strcmp(argv[i], "--help") == 0) {
            return config;
        }
    }
    
    return config;
}

std::unique_ptr<Scheduler> createScheduler(const std::string& type) {
    if (type == "RR") {
        return std::make_unique<RoundRobinScheduler>(10);
    } else if (type == "Priority") {
        return std::make_unique<PriorityScheduler>(true);
    } else if (type == "FCFS") {
        return std::make_unique<FCFSScheduler>();
    } else if (type == "DVFS") {
        return std::make_unique<DVFSScheduler>();
    }
    
    // Default to Round-Robin
    return std::make_unique<RoundRobinScheduler>(10);
}

SimulationStats runSingleSimulation(const SimulationConfig& config, const std::string& scheduler_type) {
    if (config.verbose) {
        std::cout << "Running simulation with " << scheduler_type << " scheduler..." << std::endl;
    }
    
    // Create scheduler
    auto scheduler = createScheduler(scheduler_type);
    
    // Create simulation engine
    SimulationEngine engine(std::move(scheduler));
    
    // Generate tasks
    TaskGenerator generator(config.seed);
    auto tasks = generator.generateTasks(config.num_tasks, 
                                       config.min_task_duration, 
                                       config.max_task_duration);
    
    if (config.verbose) {
        std::cout << "Generated " << tasks.size() << " tasks" << std::endl;
    }
    
    // Run simulation
    return engine.runSimulation(tasks, config.simulation_duration);
}

int main(int argc, char* argv[]) {
    std::cout << "⚡ PowerProfile: Energy-Aware Scheduling Simulator\n" << std::endl;
    
    SimulationConfig config = parseArguments(argc, argv);
    
    // Check for help
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printUsage(argv[0]);
        return 0;
    }
    
    try {
        if (config.compare_schedulers) {
            // Compare all schedulers
            std::vector<std::string> schedulers = {"RR", "Priority", "FCFS", "DVFS"};
            std::vector<SimulationStats> results;
            
            for (const auto& sched : schedulers) {
                auto stats = runSingleSimulation(config, sched);
                results.push_back(stats);
                
                if (config.verbose) {
                    ReportGenerator::printConsoleReport(stats);
                }
            }
            
            ReportGenerator::compareResults(results);
            
            // Save comparison to CSV if requested
            if (!config.output_file.empty()) {
                std::ofstream comp_file(config.output_file);
                if (comp_file.is_open()) {
                    comp_file << "scheduler,total_tasks,completed_tasks,execution_time_ticks,total_energy_J,average_power_W,peak_temp_C\n";
                    for (const auto& stat : results) {
                        comp_file << stat.scheduler_name << ","
                                 << stat.total_tasks << ","
                                 << stat.completed_tasks.size() << ","
                                 << stat.total_execution_time << ","
                                 << std::fixed << std::setprecision(6) << stat.total_energy_joules << ","
                                 << std::fixed << std::setprecision(3) << stat.average_power_watts << ","
                                 << std::fixed << std::setprecision(2) << stat.peak_temperature_celsius << "\n";
                    }
                    comp_file.close();
                    std::cout << "Comparison results saved to: " << config.output_file << std::endl;
                }
            }
        } else {
            // Single scheduler simulation
            auto stats = runSingleSimulation(config, config.scheduler_type);
            
            // Print results
            ReportGenerator::printConsoleReport(stats);
            
            // Save to CSV if requested
            if (!config.output_file.empty()) {
                if (ReportGenerator::generateCSVReport(stats, config.output_file)) {
                    std::cout << "Results saved to: " << config.output_file << std::endl;
                }
                
                // Also save detailed task CSV
                std::string task_file = config.output_file;
                size_t dot_pos = task_file.find_last_of('.');
                if (dot_pos != std::string::npos) {
                    task_file.insert(dot_pos, "_tasks");
                } else {
                    task_file += "_tasks.csv";
                }
                
                if (ReportGenerator::generateTaskCSV(stats, task_file)) {
                    std::cout << "Task details saved to: " << task_file << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
