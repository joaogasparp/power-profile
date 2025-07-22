cmake_minimum_required(VERSION 3.16)
project(PowerProfile VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(src)

# Collect source files
file(GLOB_RECURSE SOURCES 
    "src/core/*.cpp"
    "src/scheduler/*.cpp" 
    "src/models/*.cpp"
    "src/tasks/*.cpp"
    "src/output/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "src/core/*.h"
    "src/scheduler/*.h"
    "src/models/*.h" 
    "src/tasks/*.h"
    "src/output/*.h"
)

# Create executable
add_executable(powerprofile src/main.cpp ${SOURCES})

# Link libraries
target_link_libraries(powerprofile PRIVATE Threads::Threads)

# Enable testing
enable_testing()

# Find Google Test (optional - will build without if not found)
find_package(GTest QUIET)
if(GTest_FOUND)
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    add_executable(powerprofile_tests ${TEST_SOURCES} ${SOURCES})
    target_link_libraries(powerprofile_tests PRIVATE gtest_main Threads::Threads)
    add_test(NAME PowerProfileTests COMMAND powerprofile_tests)
endif()

# Installation
install(TARGETS powerprofile DESTINATION bin)

# Create data directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data/runs)
