#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <functional>

class MemoryManager;

class GarbageCollector {
public:
    GarbageCollector(MemoryManager* manager, unsigned int interval_ms = 1000);
    ~GarbageCollector();

    void start();
    void stop();
    bool is_running() const { return running_; }

private:
    MemoryManager* manager_;
    std::unique_ptr<std::thread> collector_thread_;
    std::atomic<bool> running_;
    unsigned int interval_ms_;

    void collector_loop();
}; 