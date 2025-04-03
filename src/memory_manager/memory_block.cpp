#include "memory_block.h"
#include <cstring>
#include <algorithm>

MemoryBlock::MemoryBlock(void* start, size_t size, uint64_t id)
    : start_(start)
    , size_(size)
    , id_(id)
    , ref_count_(1) {
    // Initialize memory to zero
    std::memset(start_, 0, size_);
}

MemoryBlock::~MemoryBlock() {
    // Memory is managed by the MemoryManager
}

void MemoryBlock::increase_ref_count() {
    std::lock_guard<std::mutex> lock(mutex_);
    ++ref_count_;
}

void MemoryBlock::decrease_ref_count() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (ref_count_ > 0) {
        --ref_count_;
    }
}

bool MemoryBlock::write(const void* data, size_t size) {
    if (size > size_) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    std::copy(static_cast<const char*>(data),
              static_cast<const char*>(data) + size,
              static_cast<char*>(start_));
    return true;
}

bool MemoryBlock::read(void* data, size_t size) const {
    if (size > size_) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    std::copy(static_cast<const char*>(start_),
              static_cast<const char*>(start_) + size,
              static_cast<char*>(data));
    return true;
} 