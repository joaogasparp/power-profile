#!/bin/bash

# PowerProfile Build Script
# Builds the project and runs basic tests

set -e

echo "⚡ PowerProfile Build Script"
echo "=========================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check for required tools
echo "Checking dependencies..."
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake not found. Please install CMake.${NC}"
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo -e "${RED}Error: Make not found. Please install build-essential.${NC}"
    exit 1
fi

if ! command -v g++ &> /dev/null; then
    echo -e "${RED}Error: g++ not found. Please install g++.${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Dependencies OK${NC}"

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Building project..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Build successful!${NC}"
else
    echo -e "${RED}✗ Build failed!${NC}"
    exit 1
fi

# Run tests if available
if [ -f "./powerprofile_tests" ]; then
    echo "Running tests..."
    ./powerprofile_tests
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Tests passed!${NC}"
    else
        echo -e "${YELLOW}⚠ Some tests failed${NC}"
    fi
else
    echo -e "${YELLOW}⚠ Tests not built (Google Test not found)${NC}"
fi

# Run a quick simulation test
echo "Running quick simulation test..."
if [ -f "./powerprofile" ]; then
    ./powerprofile --scheduler RR --tasks 10 --duration 1000 --verbose
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Simulation test passed!${NC}"
    else
        echo -e "${RED}✗ Simulation test failed!${NC}"
        exit 1
    fi
else
    echo -e "${RED}✗ PowerProfile binary not found!${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}🎉 Build completed successfully!${NC}"
echo ""
echo "To run PowerProfile:"
echo "  cd build"
echo "  ./powerprofile --help"
echo ""
echo "To run benchmarks:"
echo "  python3 ../scripts/benchmark.py"
echo ""
echo "To visualize results:"
echo "  python3 ../scripts/plot_results.py results.csv"
