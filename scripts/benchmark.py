#!/usr/bin/env python3
"""
PowerProfile Benchmark Runner
Runs multiple simulations with different parameters and generates comprehensive reports
"""

import subprocess
import pandas as pd
import json
import argparse
import os
import time
from pathlib import Path

class BenchmarkRunner:
    def __init__(self, powerprofile_binary="./powerprofile"):
        self.binary = powerprofile_binary
        self.results = []
        
    def run_single_benchmark(self, scheduler, tasks, duration, seed=42):
        """Run a single benchmark configuration"""
        output_file = f"benchmark_{scheduler}_{tasks}t_{duration}d_{seed}s.csv"
        
        cmd = [
            self.binary,
            "--scheduler", scheduler,
            "--tasks", str(tasks),
            "--duration", str(duration),
            "--seed", str(seed),
            "--output", output_file
        ]
        
        print(f"Running: {' '.join(cmd)}")
        
        try:
            start_time = time.time()
            result = subprocess.run(cmd, capture_output=True, text=True, check=True)
            end_time = time.time()
            
            # Read results
            if os.path.exists(output_file):
                df = pd.read_csv(output_file)
                if not df.empty:
                    row = df.iloc[0].to_dict()
                    row['benchmark_time'] = end_time - start_time
                    row['tasks_param'] = tasks
                    row['duration_param'] = duration
                    row['seed_param'] = seed
                    self.results.append(row)
                    print(f"✓ Completed in {end_time - start_time:.2f}s")
                    os.remove(output_file)  # Clean up individual files
                    return True
        except subprocess.CalledProcessError as e:
            print(f"✗ Failed: {e}")
            return False
        except Exception as e:
            print(f"✗ Error: {e}")
            return False
        
        return False
    
    def run_parameter_sweep(self):
        """Run benchmark with parameter sweeps"""
        schedulers = ["RR", "Priority", "FCFS", "DVFS"]
        task_counts = [25, 50, 100, 200]
        durations = [5000, 10000, 20000]
        seeds = [42, 123, 456]  # Multiple seeds for statistical significance
        
        total_runs = len(schedulers) * len(task_counts) * len(durations) * len(seeds)
        current_run = 0
        
        print(f"Starting parameter sweep: {total_runs} total runs")
        print("=" * 60)
        
        for scheduler in schedulers:
            for tasks in task_counts:
                for duration in durations:
                    for seed in seeds:
                        current_run += 1
                        print(f"[{current_run}/{total_runs}] ", end="")
                        self.run_single_benchmark(scheduler, tasks, duration, seed)
        
        print("\nParameter sweep completed!")
    
    def run_scalability_test(self):
        """Test scheduler scalability with increasing task counts"""
        schedulers = ["RR", "Priority", "FCFS", "DVFS"]
        task_counts = [10, 25, 50, 100, 200, 500, 1000]
        duration = 20000
        
        print("Running scalability tests...")
        print("=" * 40)
        
        for scheduler in schedulers:
            for tasks in task_counts:
                print(f"Scalability test: {scheduler} with {tasks} tasks")
                self.run_single_benchmark(scheduler, tasks, duration)
    
    def save_results(self, filename="benchmark_results.csv"):
        """Save all benchmark results to CSV"""
        if self.results:
            df = pd.DataFrame(self.results)
            df.to_csv(filename, index=False)
            print(f"Results saved to {filename}")
            
            # Generate summary statistics
            self.generate_summary(df)
            return df
        else:
            print("No results to save")
            return None
    
    def generate_summary(self, df):
        """Generate summary statistics"""
        print("\n" + "=" * 60)
        print("BENCHMARK SUMMARY")
        print("=" * 60)
        
        # Best performer by metric
        metrics = {
            'Energy Efficiency': ('total_energy_J', False),  # Lower is better
            'Execution Speed': ('execution_time_ticks', False),  # Lower is better
            'Thermal Performance': ('peak_temp_C', False),  # Lower is better
            'Task Completion': ('completed_tasks', True)  # Higher is better
        }
        
        for metric_name, (column, higher_better) in metrics.items():
            if column in df.columns:
                if higher_better:
                    best_idx = df[column].idxmax()
                else:
                    best_idx = df[column].idxmin()
                
                best = df.loc[best_idx]
                print(f"\nBest {metric_name}:")
                print(f"  Scheduler: {best['scheduler']}")
                print(f"  Value: {best[column]:.6f}")
                print(f"  Config: {best['tasks_param']} tasks, {best['duration_param']} duration")
        
        # Average performance by scheduler
        print("\nAverage Performance by Scheduler:")
        scheduler_avg = df.groupby('scheduler').agg({
            'total_energy_J': 'mean',
            'execution_time_ticks': 'mean', 
            'peak_temp_C': 'mean',
            'completed_tasks': 'mean',
            'benchmark_time': 'mean'
        }).round(6)
        
        print(scheduler_avg)

def main():
    parser = argparse.ArgumentParser(description='Run PowerProfile benchmarks')
    parser.add_argument('--binary', '-b', default='./powerprofile', 
                       help='Path to powerprofile binary')
    parser.add_argument('--sweep', action='store_true', 
                       help='Run parameter sweep')
    parser.add_argument('--scalability', action='store_true', 
                       help='Run scalability tests')
    parser.add_argument('--output', '-o', default='benchmark_results.csv',
                       help='Output CSV file')
    parser.add_argument('--scheduler', help='Test specific scheduler only')
    parser.add_argument('--tasks', type=int, default=50, help='Number of tasks')
    parser.add_argument('--duration', type=int, default=10000, help='Simulation duration')
    
    args = parser.parse_args()
    
    # Check if binary exists
    if not os.path.exists(args.binary):
        print(f"Error: PowerProfile binary '{args.binary}' not found")
        print("Please build the project first: cmake --build build")
        return 1
    
    runner = BenchmarkRunner(args.binary)
    
    if args.sweep:
        runner.run_parameter_sweep()
    elif args.scalability:
        runner.run_scalability_test()
    elif args.scheduler:
        runner.run_single_benchmark(args.scheduler, args.tasks, args.duration)
    else:
        # Run comparison of all schedulers with default parameters
        schedulers = ["RR", "Priority", "FCFS", "DVFS"]
        for scheduler in schedulers:
            runner.run_single_benchmark(scheduler, args.tasks, args.duration)
    
    df = runner.save_results(args.output)
    
    if df is not None:
        print(f"\nTo visualize results, run:")
        print(f"python3 scripts/plot_results.py {args.output}")

if __name__ == "__main__":
    exit(main())
