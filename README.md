# ⚡ PowerProfile: Energy-Aware Scheduling Simulator

A simulation framework to compare and evaluate different CPU task scheduling algorithms with respect to **energy efficiency, task performance, and thermal impact**.

## Overview

PowerProfile models simplified CPU behavior under various scheduling policies and produces metrics on execution time, energy usage, thermal rise, and overall system efficiency. The project focuses on studying the trade-offs between **performance and energy** — critical for cloud computing, embedded systems, mobile devices, and high-performance computing.

## Features

- **Task Generator**: Creates synthetic tasks with various resource profiles
- **Scheduler Engine**: Supports plug-and-play scheduling algorithms (RR, Priority, DVFS-aware)
- **Energy Consumption Model**: Estimates based on CPU frequency, time, and workload profile
- **Thermal Model**: Simulates temperature increase over time per core
- **Visual Metrics Output**: CLI reports and CSV generation
- **Configurable Simulations**: Adjustable parameters for comprehensive testing

## Build Instructions

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./powerprofile --scheduler RR --tasks 100 --duration 10000
./powerprofile --scheduler DVFS --tasks 50 --duration 5000 --output results.csv
```

## Supported Schedulers

- **RR**: Round-Robin
- **FCFS**: First-Come-First-Serve
- **Priority**: Priority-based scheduling
- **DVFS**: Dynamic Voltage/Frequency Scaling aware
- **Custom**: Energy-optimized custom policy

## Output Example

```
[SIMULATION COMPLETE]
Total Tasks: 100
Total Execution Time: 9500 ticks
Average Power: 12.4 W
Peak Temp: 72.1 °C
Scheduler: DVFS-Aware
```

## Project Structure

```
powerprofile-simulator/
├── src/              # Source code
│   ├── core/         # Simulation engine
│   ├── scheduler/    # Scheduling algorithms
│   ├── models/       # Energy and thermal models
│   ├── tasks/        # Task generation
│   └── output/       # Report and logging
├── tests/            # Unit tests
├── scripts/          # Python plotting utilities
└── data/runs/        # CSV output files
```

## License

MIT License - see LICENSE file for details.
