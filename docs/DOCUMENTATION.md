# PowerProfile Documentation

## Project Overview

PowerProfile is an energy-aware CPU task scheduling simulator designed for research and educational purposes in the field of computer engineering and systems optimization.

## Architecture

### Core Components

1. **Task System** (`src/tasks/`)
   - `Task.h/cpp`: Individual task representation
   - `TaskGenerator.h/cpp`: Synthetic task generation with configurable distributions

2. **Schedulers** (`src/scheduler/`)
   - `Scheduler.h`: Abstract base class for all schedulers
   - `RoundRobinScheduler.h/cpp`: Time-sliced round-robin scheduling
   - `PriorityScheduler.h/cpp`: Priority-based preemptive/non-preemptive scheduling
   - `FCFSScheduler.h/cpp`: First-Come-First-Serve scheduling
   - `DVFSScheduler.h/cpp`: Dynamic Voltage/Frequency Scaling aware scheduler

3. **Models** (`src/models/`)
   - `EnergyModel.h/cpp`: Power and energy consumption calculation
   - `ThermalModel.h/cpp`: Temperature simulation and thermal throttling

4. **Simulation Engine** (`src/core/`)
   - `SimulationEngine.h/cpp`: Main simulation orchestrator

5. **Output System** (`src/output/`)
   - `ReportGenerator.h/cpp`: Console and CSV report generation

## Scheduling Algorithms

### Round-Robin (RR)
- **Time Quantum**: 10 ticks (configurable)
- **Characteristics**: Fair time sharing, prevents starvation
- **Best for**: Interactive workloads, general-purpose systems

### Priority-Based
- **Mode**: Preemptive priority scheduling
- **Priority Range**: 1-10 (higher number = higher priority)
- **Characteristics**: High-priority tasks execute first
- **Best for**: Real-time systems, critical task prioritization

### First-Come-First-Serve (FCFS)
- **Mode**: Non-preemptive
- **Characteristics**: Simple, no context switching overhead
- **Best for**: Batch processing, compute-intensive workloads

### DVFS-Aware
- **Frequency Range**: 0.5 - 2.0 GHz (6 levels)
- **Characteristics**: Dynamic frequency scaling based on task type and system load
- **Best for**: Energy-constrained systems, mobile devices

## Energy Model

The energy model uses a simplified CMOS power equation:

```
P = C × V² × f × α
```

Where:
- `C` = Effective capacitance (configurable, default: 1nF)
- `V` = Supply voltage (scales with frequency)
- `f` = Operating frequency (GHz)
- `α` = Activity factor (depends on task type)

### Task Type Multipliers
- **CPU_BOUND**: 1.2x (high computational activity)
- **IO_BOUND**: 0.6x (low CPU utilization during I/O waits)
- **MEMORY_HEAVY**: 0.9x (moderate CPU usage, memory access overhead)

## Thermal Model

Simplified discrete thermal model:

```
T[t] = T[t-1] + k×P - d×(T[t-1] - T_ambient)
```

Where:
- `k` = Heating coefficient (default: 0.1)
- `d` = Cooling coefficient (default: 0.05)
- `T_ambient` = Ambient temperature (default: 25°C)

## Usage Examples

### Basic Simulation
```bash
# Run Round-Robin with 50 tasks
./powerprofile --scheduler RR --tasks 50 --duration 10000

# Run Priority scheduler with output
./powerprofile --scheduler Priority --tasks 100 --output results.csv

# Compare all schedulers
./powerprofile --compare --tasks 75
```

### Advanced Usage
```bash
# Custom task parameters
./powerprofile --scheduler DVFS --tasks 200 --duration 20000 \
  --min-duration 25 --max-duration 1000 --seed 123

# Verbose output for debugging
./powerprofile --scheduler RR --tasks 10 --verbose
```

### Benchmarking
```bash
# Run comprehensive benchmarks
python3 scripts/benchmark.py --sweep

# Test scalability
python3 scripts/benchmark.py --scalability

# Single scheduler benchmark
python3 scripts/benchmark.py --scheduler DVFS --tasks 500
```

### Visualization
```bash
# Plot comparison results
python3 scripts/plot_results.py results.csv

# Plot task-level analysis
python3 scripts/plot_results.py task_results.csv --tasks
```

## Metrics and Analysis

### Primary Metrics
- **Energy Consumption (J)**: Total energy consumed during simulation
- **Execution Time (ticks)**: Total simulation time to complete all tasks
- **Peak Temperature (°C)**: Maximum temperature reached
- **Average Power (W)**: Mean power consumption
- **Completed Tasks**: Number of tasks finished within time limit

### Secondary Metrics
- **Turnaround Time**: Time from task arrival to completion
- **Task Distribution**: Completion rate by task type
- **Thermal Throttling**: Temperature-based performance degradation events

## Configuration and Tuning

### Energy Model Parameters
```cpp
EnergyModel model(1e-9, 1.2);  // capacitance, base_voltage
model.setTaskTypeMultipliers(1.2, 0.6, 0.9);
```

### Thermal Model Parameters
```cpp
ThermalModel thermal(25.0, 0.1, 0.05);  // ambient, heating_coeff, cooling_coeff
```

### Task Generation
```cpp
TaskGenerator generator(42);  // seed
generator.setTaskTypeDistribution(0.4, 0.3, 0.3);  // CPU, IO, Memory
generator.setPriorityRange(1, 10);
```

## Research Applications

1. **Energy Efficiency Studies**: Compare energy consumption across different scheduling policies
2. **Thermal Management**: Analyze temperature behavior and throttling impacts
3. **Workload Characterization**: Study performance with different task distributions
4. **Algorithm Development**: Test custom scheduling algorithms
5. **System Optimization**: Find optimal parameters for specific workloads

## Extending the Simulator

### Adding New Schedulers
1. Inherit from `Scheduler` base class
2. Implement required virtual methods:
   - `addTask()`, `getNextTask()`, `taskCompleted()`, `tick()`, `reset()`
3. Add to scheduler factory in `main.cpp`

### Custom Energy Models
1. Inherit from `EnergyModel` or modify existing implementation
2. Override `calculatePower()` and `calculateTaskEnergy()` methods
3. Integrate with `SimulationEngine`

### Additional Metrics
1. Extend `SimulationStats` structure
2. Update `ReportGenerator` for new output formats
3. Modify visualization scripts for new metrics

## Validation and Testing

The simulator includes comprehensive unit tests covering:
- Task execution and state management
- Scheduler functionality and fairness
- Energy model accuracy and scaling
- Thermal model stability and throttling

Run tests with:
```bash
cd build
./powerprofile_tests
```

## Performance Considerations

- **Simulation Granularity**: 1 tick = 1ms simulation time
- **Memory Usage**: Scales with number of active tasks
- **Computation Time**: Linear with simulation duration and task count
- **Accuracy vs Speed**: Trade-off between model fidelity and execution time

## Limitations and Future Work

### Current Limitations
- Single-core simulation only
- Simplified energy and thermal models
- No cache or memory hierarchy modeling
- Limited I/O simulation

### Potential Extensions
- Multi-core and NUMA support
- Real hardware power traces integration
- Advanced thermal modeling with heat spreading
- Network and storage I/O simulation
- Dynamic workload arrival patterns
