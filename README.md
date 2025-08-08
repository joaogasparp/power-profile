# PowerProfile: Energy-Aware Scheduling Simulator

A simulation framework to compare and evaluate different CPU task scheduling algorithms with respect to **energy efficiency, task performance, and thermal impact**.

## Overview

PowerProfile models simplified CPU behavior under various scheduling policies and produces metrics on execution time, energy usage, thermal rise, and overall system efficiency. The project focuses on studying the trade-offs between **performance and energy**: critical for cloud computing, embedded systems, mobile devices, and high-performance computing.

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

1. Run the simulator:

```bash
./powerprofile --help
```

2. To visualize the results:

```bash
python3 scripts/plot_results.py results.csv
```
