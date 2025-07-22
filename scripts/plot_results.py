#!/usr/bin/env python3
"""
PowerProfile Results Visualizer
Generates plots from simulation results CSV files
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import argparse
import sys
import os

def load_results(csv_file):
    """Load simulation results from CSV file"""
    try:
        df = pd.read_csv(csv_file)
        return df
    except FileNotFoundError:
        print(f"Error: File '{csv_file}' not found")
        return None
    except pd.errors.EmptyDataError:
        print(f"Error: File '{csv_file}' is empty")
        return None

def plot_comparison(df, output_dir="plots"):
    """Create comparison plots for different schedulers"""
    os.makedirs(output_dir, exist_ok=True)
    
    plt.style.use('seaborn-v0_8')
    
    # 1. Energy consumption comparison
    plt.figure(figsize=(10, 6))
    bars = plt.bar(df['scheduler'], df['total_energy_J'], color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728'])
    plt.title('Energy Consumption by Scheduler', fontsize=14, fontweight='bold')
    plt.xlabel('Scheduler')
    plt.ylabel('Total Energy (J)')
    plt.xticks(rotation=45)
    
    # Add value labels on bars
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.6f}', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/energy_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 2. Execution time comparison
    plt.figure(figsize=(10, 6))
    bars = plt.bar(df['scheduler'], df['execution_time_ticks'], color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728'])
    plt.title('Execution Time by Scheduler', fontsize=14, fontweight='bold')
    plt.xlabel('Scheduler')
    plt.ylabel('Execution Time (ticks)')
    plt.xticks(rotation=45)
    
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{int(height)}', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/execution_time_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 3. Peak temperature comparison
    plt.figure(figsize=(10, 6))
    bars = plt.bar(df['scheduler'], df['peak_temp_C'], color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728'])
    plt.title('Peak Temperature by Scheduler', fontsize=14, fontweight='bold')
    plt.xlabel('Scheduler')
    plt.ylabel('Peak Temperature (°C)')
    plt.xticks(rotation=45)
    
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f}°C', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/temperature_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 4. Multi-metric comparison (radar chart)
    fig, ax = plt.subplots(figsize=(10, 10), subplot_kw=dict(projection='polar'))
    
    # Normalize metrics for radar chart (0-1 scale)
    metrics = ['total_energy_J', 'execution_time_ticks', 'peak_temp_C', 'average_power_W']
    metric_labels = ['Energy', 'Exec Time', 'Peak Temp', 'Avg Power']
    
    # Normalize each metric (higher is worse, so we invert for better visualization)
    normalized_df = df.copy()
    for metric in metrics:
        max_val = df[metric].max()
        min_val = df[metric].min()
        if max_val != min_val:
            normalized_df[metric] = 1 - (df[metric] - min_val) / (max_val - min_val)
        else:
            normalized_df[metric] = 1.0
    
    angles = [n / len(metrics) * 2 * 3.14159 for n in range(len(metrics))]
    angles += angles[:1]  # Complete the circle
    
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']
    
    for i, scheduler in enumerate(df['scheduler']):
        values = normalized_df.iloc[i][metrics].tolist()
        values += values[:1]  # Complete the circle
        
        ax.plot(angles, values, 'o-', linewidth=2, label=scheduler, color=colors[i])
        ax.fill(angles, values, alpha=0.25, color=colors[i])
    
    ax.set_xticks(angles[:-1])
    ax.set_xticklabels(metric_labels)
    ax.set_ylim(0, 1)
    ax.set_title('Multi-Metric Performance Comparison\n(Higher = Better)', size=14, fontweight='bold', pad=20)
    ax.legend(loc='upper right', bbox_to_anchor=(0.1, 0.1))
    ax.grid(True)
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/radar_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    print(f"Plots saved in '{output_dir}/' directory:")
    print("  - energy_comparison.png")
    print("  - execution_time_comparison.png") 
    print("  - temperature_comparison.png")
    print("  - radar_comparison.png")

def plot_tasks(csv_file, output_dir="plots"):
    """Create task-level analysis plots"""
    df = load_results(csv_file)
    if df is None:
        return
    
    os.makedirs(output_dir, exist_ok=True)
    
    # Task duration distribution by type
    plt.figure(figsize=(12, 6))
    task_types = df['type'].unique()
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c']
    
    for i, task_type in enumerate(task_types):
        type_data = df[df['type'] == task_type]['duration']
        plt.hist(type_data, alpha=0.7, label=task_type, bins=20, color=colors[i % len(colors)])
    
    plt.title('Task Duration Distribution by Type', fontsize=14, fontweight='bold')
    plt.xlabel('Duration (ticks)')
    plt.ylabel('Count')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'{output_dir}/task_duration_distribution.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # Turnaround time by task type
    if 'turnaround_time' in df.columns:
        plt.figure(figsize=(10, 6))
        df_valid = df[df['turnaround_time'] > 0]
        sns.boxplot(data=df_valid, x='type', y='turnaround_time')
        plt.title('Turnaround Time by Task Type', fontsize=14, fontweight='bold')
        plt.xlabel('Task Type')
        plt.ylabel('Turnaround Time (ticks)')
        plt.xticks(rotation=45)
        plt.tight_layout()
        plt.savefig(f'{output_dir}/turnaround_by_type.png', dpi=300, bbox_inches='tight')
        plt.close()
        
        print(f"Task analysis plots saved in '{output_dir}/' directory:")
        print("  - task_duration_distribution.png")
        print("  - turnaround_by_type.png")

def main():
    parser = argparse.ArgumentParser(description='Visualize PowerProfile simulation results')
    parser.add_argument('csv_file', help='CSV file with simulation results')
    parser.add_argument('--output-dir', '-o', default='plots', help='Output directory for plots')
    parser.add_argument('--tasks', action='store_true', help='Plot task-level analysis (requires tasks CSV)')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.csv_file):
        print(f"Error: File '{args.csv_file}' not found")
        sys.exit(1)
    
    df = load_results(args.csv_file)
    if df is None:
        sys.exit(1)
    
    if args.tasks:
        plot_tasks(args.csv_file, args.output_dir)
    else:
        plot_comparison(df, args.output_dir)

if __name__ == "__main__":
    main()
