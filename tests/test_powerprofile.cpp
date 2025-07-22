#include <gtest/gtest.h>
#include "tasks/Task.h"
#include "tasks/TaskGenerator.h"
#include "scheduler/RoundRobinScheduler.h"
#include "models/EnergyModel.h"
#include "models/ThermalModel.h"

// Task tests
TEST(TaskTest, BasicFunctionality) {
    Task task(1, 100, TaskType::CPU_BOUND, 5);
    
    EXPECT_EQ(task.getId(), 1);
    EXPECT_EQ(task.getDuration(), 100);
    EXPECT_EQ(task.getRemainingTime(), 100);
    EXPECT_EQ(task.getType(), TaskType::CPU_BOUND);
    EXPECT_EQ(task.getPriority(), 5);
    EXPECT_FALSE(task.isCompleted());
}

TEST(TaskTest, Execution) {
    Task task(1, 100, TaskType::CPU_BOUND);
    
    task.execute(30);
    EXPECT_EQ(task.getRemainingTime(), 70);
    EXPECT_FALSE(task.isCompleted());
    
    task.execute(70);
    EXPECT_EQ(task.getRemainingTime(), 0);
    EXPECT_TRUE(task.isCompleted());
}

TEST(TaskTest, OverExecution) {
    Task task(1, 50, TaskType::CPU_BOUND);
    
    task.execute(100); // Execute more than duration
    EXPECT_EQ(task.getRemainingTime(), 0);
    EXPECT_TRUE(task.isCompleted());
}

// TaskGenerator tests
TEST(TaskGeneratorTest, GenerateBasicTasks) {
    TaskGenerator generator(42);
    auto tasks = generator.generateTasks(10, 50, 200);
    
    EXPECT_EQ(tasks.size(), 10);
    
    for (const auto& task : tasks) {
        EXPECT_GE(task.getDuration(), 50);
        EXPECT_LE(task.getDuration(), 200);
        EXPECT_GE(task.getPriority(), 1);
        EXPECT_LE(task.getPriority(), 10);
    }
}

TEST(TaskGeneratorTest, TaskTypeDistribution) {
    TaskGenerator generator(42);
    generator.setTaskTypeDistribution(1.0, 0.0, 0.0); // Only CPU_BOUND
    
    auto tasks = generator.generateTasks(20, 50, 100);
    
    for (const auto& task : tasks) {
        EXPECT_EQ(task.getType(), TaskType::CPU_BOUND);
    }
}

// RoundRobinScheduler tests
TEST(RoundRobinSchedulerTest, BasicScheduling) {
    RoundRobinScheduler scheduler(10);
    
    Task task1(1, 50, TaskType::CPU_BOUND);
    Task task2(2, 30, TaskType::IO_BOUND);
    
    scheduler.addTask(task1);
    scheduler.addTask(task2);
    
    EXPECT_TRUE(scheduler.hasTasks());
    EXPECT_EQ(scheduler.getActiveTasksCount(), 2);
    
    Task* current = scheduler.getNextTask();
    EXPECT_NE(current, nullptr);
    EXPECT_EQ(current->getId(), 1);
}

// EnergyModel tests
TEST(EnergyModelTest, PowerCalculation) {
    EnergyModel model;
    
    double power_cpu = model.calculatePower(TaskType::CPU_BOUND, 1.0);
    double power_io = model.calculatePower(TaskType::IO_BOUND, 1.0);
    double power_mem = model.calculatePower(TaskType::MEMORY_HEAVY, 1.0);
    
    EXPECT_GT(power_cpu, 0.0);
    EXPECT_GT(power_io, 0.0);
    EXPECT_GT(power_mem, 0.0);
    
    // CPU-bound should consume more power than I/O-bound
    EXPECT_GT(power_cpu, power_io);
}

TEST(EnergyModelTest, FrequencyScaling) {
    EnergyModel model;
    
    double power_low = model.calculatePower(TaskType::CPU_BOUND, 0.5);
    double power_high = model.calculatePower(TaskType::CPU_BOUND, 2.0);
    
    // Higher frequency should result in higher power consumption
    EXPECT_GT(power_high, power_low);
}

// ThermalModel tests
TEST(ThermalModelTest, TemperatureUpdate) {
    ThermalModel model(25.0, 0.1, 0.05);
    
    EXPECT_EQ(model.getCurrentTemperature(), 25.0);
    EXPECT_EQ(model.getPeakTemperature(), 25.0);
    
    // Apply some power and check temperature rise
    model.updateTemperature(10.0); // 10W
    EXPECT_GT(model.getCurrentTemperature(), 25.0);
    EXPECT_GT(model.getPeakTemperature(), 25.0);
}

TEST(ThermalModelTest, ThermalThrottling) {
    ThermalModel model(25.0, 0.5, 0.01); // High heating, low cooling
    
    // Heat up the system
    for (int i = 0; i < 100; ++i) {
        model.updateTemperature(20.0); // High power
    }
    
    EXPECT_TRUE(model.shouldThrottle(85.0));
}

TEST(ThermalModelTest, Reset) {
    ThermalModel model(25.0, 0.1, 0.05);
    
    model.updateTemperature(10.0);
    EXPECT_GT(model.getCurrentTemperature(), 25.0);
    
    model.reset();
    EXPECT_EQ(model.getCurrentTemperature(), 25.0);
    EXPECT_EQ(model.getPeakTemperature(), 25.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
