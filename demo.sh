#!/bin/bash

# PowerProfile Demo Script
# Comprehensive demonstration of the simulator capabilities

set -e

echo "⚡ PowerProfile: Energy-Aware Scheduling Simulator Demo"
echo "======================================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

cd build

echo -e "${BLUE}1. Single Scheduler Demonstration - Round-Robin${NC}"
echo "Running Round-Robin scheduler with 30 tasks..."
./powerprofile --scheduler RR --tasks 30 --duration 8000 --output ../demo_results/rr_demo.csv

echo ""
echo -e "${BLUE}2. Energy-Aware DVFS Scheduler Demonstration${NC}"
echo "Running DVFS scheduler with dynamic frequency scaling..."
./powerprofile --scheduler DVFS --tasks 40 --duration 10000 --output ../demo_results/dvfs_demo.csv --verbose

echo ""
echo -e "${BLUE}3. Comprehensive Scheduler Comparison${NC}"
echo "Comparing all available scheduling algorithms..."
./powerprofile --compare --tasks 100 --duration 15000 --output ../demo_results/full_comparison.csv

echo ""
echo -e "${BLUE}4. Scalability Test${NC}"
echo "Testing with increasing task loads..."
for tasks in 25 50 100 200; do
    echo "  Testing with $tasks tasks..."
    ./powerprofile --scheduler Priority --tasks $tasks --duration 15000 --output ../demo_results/scale_${tasks}tasks.csv
done

cd ..

echo ""
echo -e "${BLUE}5. Data Visualization${NC}"
echo "Generating comparison plots..."
python3 scripts/plot_results.py demo_results/full_comparison.csv --output-dir demo_plots

echo "Generating task analysis plots..."
python3 scripts/plot_results.py demo_results/dvfs_demo_tasks.csv --tasks --output-dir demo_plots

echo ""
echo -e "${GREEN}🎉 Demo completed successfully!${NC}"
echo ""
echo -e "${YELLOW}Results Summary:${NC}"
echo "📁 CSV Results: demo_results/"
echo "📊 Visualization: demo_plots/"
echo ""
echo -e "${YELLOW}Key Files Created:${NC}"
ls -la demo_results/ | head -10
echo ""
ls -la demo_plots/ | head -10

echo ""
echo -e "${BLUE}Next Steps:${NC}"
echo "• Explore the generated CSV files for detailed metrics"
echo "• View the PNG plots for visual comparisons" 
echo "• Run custom simulations with different parameters"
echo "• Extend the simulator with your own scheduling algorithms"
echo ""
echo "For more information, see docs/DOCUMENTATION.md"
