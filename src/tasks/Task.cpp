#include "Task.h"

Task::Task(int id, int duration, TaskType type, int priority)
    : id_(id), duration_(duration), remaining_time_(duration), 
      type_(type), priority_(priority), start_time_(-1), end_time_(-1) {
}

void Task::execute(int time_slice) {
    remaining_time_ -= time_slice;
    if (remaining_time_ < 0) {
        remaining_time_ = 0;
    }
}

void Task::reset() {
    remaining_time_ = duration_;
    start_time_ = -1;
    end_time_ = -1;
}

std::string Task::getTypeString() const {
    switch (type_) {
        case TaskType::CPU_BOUND: return "CPU_BOUND";
        case TaskType::IO_BOUND: return "IO_BOUND";
        case TaskType::MEMORY_HEAVY: return "MEMORY_HEAVY";
        default: return "UNKNOWN";
    }
}
