#include "garbage_collector.h"
#include "memory_manager.h"
#include <chrono>

GarbageCollector::GarbageCollector(MemoryManager* manager, unsigned int interval_ms)
    : manager_(manager)
    , running_(false)
    , interval_ms_(interval_ms) {}

GarbageCollector::~GarbageCollector() {
    stop();
}

void GarbageCollector::start() {
    if (!running_) {
        running_ = true;
        collector_thread_ = std::make_unique<std::thread>(&GarbageCollector::collector_loop, this);
    }
}

void GarbageCollector::stop() {
    if (running_) {
        running_ = false;
        if (collector_thread_ && collector_thread_->joinable()) {
            collector_thread_->join();
        }
    }
}

void GarbageCollector::collector_loop() {
    while (running_) {
        // Sleep for the specified interval
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms_));
        
        // Check for blocks with zero reference count
        manager_->defragment();
    }
} 