#pragma once

#include <cstdint>
#include <memory>
#include <mutex>

class MemoryBlock {
public:
    MemoryBlock(void* start, size_t size, uint64_t id);
    ~MemoryBlock();

    // Getters
    void* get_start() const { return start_; }
    size_t get_size() const { return size_; }
    uint64_t get_id() const { return id_; }
    uint32_t get_ref_count() const { return ref_count_; }

    // Setters
    void set_start(void* new_start) { start_ = new_start; }

    // Reference counting
    void increase_ref_count();
    void decrease_ref_count();
    bool is_free() const { return ref_count_ == 0; }

    // Memory operations
    bool write(const void* data, size_t size);
    bool read(void* data, size_t size) const;

private:
    void* start_;
    size_t size_;
    uint64_t id_;
    uint32_t ref_count_;
    mutable std::mutex mutex_;
}; 