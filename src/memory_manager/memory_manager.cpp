#include "memory_manager.h"
#include "../proto/memory_service.grpc.pb.h"
#include "../proto/memory_service.pb.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

MemoryManager::MemoryManager(size_t memory_size, const std::string& dump_folder)
    : memory_block_(malloc(memory_size))
    , total_size_(memory_size)
    , used_size_(0)
    , dump_folder_(dump_folder)
    , next_id_(1)
    , garbage_collector_(std::make_unique<GarbageCollector>(this)) {
    if (!memory_block_) {
        throw std::runtime_error("Failed to allocate memory block");
    }
    garbage_collector_->start();
}

MemoryManager::~MemoryManager() {
    garbage_collector_->stop();
    free(memory_block_);
}

grpc::Status MemoryManager::Create(grpc::ServerContext* context,
                                 const memory_service::CreateRequest* request,
                                 memory_service::CreateResponse* response) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (is_fragmented()) {
        optimize_memory();
    }
    
    size_t type_size = get_type_size(request->type());
    size_t total_size = request->size() * type_size;
    
    if (used_size_ + total_size > total_size_) {
        response->set_success(false);
        response->set_error_message("Not enough memory available");
        return grpc::Status::OK;
    }
    
    void* block_start = static_cast<char*>(memory_block_) + used_size_;
    uint64_t id = next_id_++;
    
    blocks_[id] = std::make_unique<MemoryBlock>(block_start, total_size, id);
    used_size_ += total_size;
    
    response->set_id(id);
    response->set_success(true);
    
    generate_dump();
    log_memory_operation("Create", id, total_size);
    return grpc::Status::OK;
}

grpc::Status MemoryManager::Set(grpc::ServerContext* context,
                              const memory_service::SetRequest* request,
                              memory_service::SetResponse* response) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!is_valid_id(request->id())) {
        response->set_success(false);
        response->set_error_message("Invalid memory block ID");
        return grpc::Status::OK;
    }
    
    auto& block = blocks_[request->id()];
    if (!block->write(request->value().data(), request->value().size())) {
        response->set_success(false);
        response->set_error_message("Failed to write to memory block");
        return grpc::Status::OK;
    }
    
    response->set_success(true);
    generate_dump();
    return grpc::Status::OK;
}

grpc::Status MemoryManager::Get(grpc::ServerContext* context,
                              const memory_service::GetRequest* request,
                              memory_service::GetResponse* response) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!is_valid_id(request->id())) {
        response->set_success(false);
        response->set_error_message("Invalid memory block ID");
        return grpc::Status::OK;
    }
    
    auto& block = blocks_[request->id()];
    std::string value(block->get_size(), '\0');
    
    if (!block->read(value.data(), value.size())) {
        response->set_success(false);
        response->set_error_message("Failed to read from memory block");
        return grpc::Status::OK;
    }
    
    response->set_value(value);
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status MemoryManager::IncreaseRefCount(grpc::ServerContext* context,
                                           const memory_service::RefCountRequest* request,
                                           memory_service::RefCountResponse* response) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!is_valid_id(request->id())) {
        response->set_success(false);
        response->set_error_message("Invalid memory block ID");
        return grpc::Status::OK;
    }
    
    blocks_[request->id()]->increase_ref_count();
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status MemoryManager::DecreaseRefCount(grpc::ServerContext* context,
                                           const memory_service::RefCountRequest* request,
                                           memory_service::RefCountResponse* response) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!is_valid_id(request->id())) {
        response->set_success(false);
        response->set_error_message("Invalid memory block ID");
        return grpc::Status::OK;
    }
    
    blocks_[request->id()]->decrease_ref_count();
    response->set_success(true);
    return grpc::Status::OK;
}

void MemoryManager::defragment() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Collect blocks to be removed (ref_count == 0)
    std::vector<uint64_t> blocks_to_remove;
    for (const auto& pair : blocks_) {
        if (pair.second->is_free()) {
            blocks_to_remove.push_back(pair.first);
        }
    }
    
    if (blocks_to_remove.empty()) {
        return;
    }
    
    // Sort blocks by memory address
    std::vector<std::pair<void*, MemoryBlock*>> sorted_blocks;
    for (const auto& pair : blocks_) {
        sorted_blocks.emplace_back(pair.second->get_start(), pair.second.get());
    }
    std::sort(sorted_blocks.begin(), sorted_blocks.end());
    
    // Compact memory by moving blocks
    char* current = static_cast<char*>(memory_block_);
    for (const auto& pair : sorted_blocks) {
        MemoryBlock* block = pair.second;
        if (!block->is_free()) {
            if (block->get_start() != current) {
                std::memmove(current, block->get_start(), block->get_size());
                block->set_start(current);
            }
            current += block->get_size();
        }
    }
    
    // Update used size
    used_size_ = current - static_cast<char*>(memory_block_);
    
    // Remove freed blocks
    for (uint64_t id : blocks_to_remove) {
        blocks_.erase(id);
    }
    
    generate_dump();
}

void MemoryManager::generate_dump() {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << dump_folder_ << "/dump_"
       << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S")
       << "_" << std::setfill('0') << std::setw(3) << now_ms.count()
       << ".bin";
    
    std::ofstream file(ss.str(), std::ios::binary);
    if (file) {
        file.write(static_cast<char*>(memory_block_), used_size_);
    }
}

bool MemoryManager::is_valid_id(uint64_t id) {
    return blocks_.find(id) != blocks_.end();
}

size_t MemoryManager::get_type_size(memory_service::DataType type) {
    switch (type) {
        case memory_service::INT:
            return sizeof(int);
        case memory_service::FLOAT:
            return sizeof(float);
        case memory_service::DOUBLE:
            return sizeof(double);
        case memory_service::CHAR:
            return sizeof(char);
        case memory_service::BOOL:
            return sizeof(bool);
        case memory_service::CUSTOM:
            return sizeof(void*);  // For custom types, use pointer size
        default:
            throw std::runtime_error("Unsupported data type");
    }
}

MemoryManager::MemoryStats MemoryManager::get_memory_stats() const {
    std::lock_guard<std::mutex> lock(mutex_);
    // Update the memory stats structure with current values for backward compatibility
    memory_stats_.total_memory = total_size_;
    memory_stats_.used_memory = used_size_;
    memory_stats_.free_memory = total_size_ - used_size_;
    memory_stats_.num_allocations = blocks_.size();
    return memory_stats_;
}

bool MemoryManager::is_fragmented() const {
    std::lock_guard<std::mutex> lock(mutex_);
    // Consider memory fragmented if fragmentation percentage is above 10%
    return memory_stats_.fragmentation_percentage > 10.0f;
}

void MemoryManager::optimize_memory() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (should_defragment()) {
        defragment();
    }
    merge_adjacent_blocks();
    update_memory_stats();
}

void MemoryManager::update_memory_stats() {
    std::lock_guard<std::mutex> lock(mutex_);
    memory_stats_.total_memory = total_size_;
    memory_stats_.used_memory = used_size_;
    memory_stats_.free_memory = total_size_ - used_size_;
    memory_stats_.num_allocations = blocks_.size();
    memory_stats_.fragmentation_percentage = 0;
    
    if (!blocks_.empty()) {
        // Sort blocks by address
        std::vector<MemoryBlock*> sorted_blocks;
        for (auto& pair : blocks_) {
            sorted_blocks.push_back(pair.second.get());
        }
        std::sort(sorted_blocks.begin(), sorted_blocks.end(), 
                 [](const MemoryBlock* a, const MemoryBlock* b) {
                     return a->get_start() < b->get_start();
                 });
        
        // Calculate fragmentation by measuring gaps between blocks
        char* last_end = static_cast<char*>(memory_block_);
        size_t total_gaps = 0;
        
        for (auto* block : sorted_blocks) {
            // Cast void* to char* for pointer arithmetic
            char* block_start = static_cast<char*>(block->get_start());
            size_t gap = block_start - last_end;
            if (gap > 0) {
                total_gaps += gap;
            }
            last_end = block_start + block->get_size();
        }
        
        // Check for gap at the end
        char* memory_end = static_cast<char*>(memory_block_) + total_size_;
        size_t end_gap = memory_end - last_end;
        if (end_gap > 0) {
            total_gaps += end_gap;
        }
        
        if (memory_stats_.free_memory > 0) {
            memory_stats_.fragmentation_percentage = 
                static_cast<float>(total_gaps) / static_cast<float>(memory_stats_.free_memory) * 100.0f;
        }
    }
}

bool MemoryManager::should_defragment() const {
    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()).count() - last_defrag_time_;
    
    // Defragment if fragmentation percentage is above 10% and it's been more than 5 minutes
    return memory_stats_.fragmentation_percentage > 10.0f && elapsed > 300;
}

void MemoryManager::compact_memory() {
    char* current = static_cast<char*>(memory_block_);
    for (const auto& pair : blocks_) {
        auto& block = pair.second;
        if (!block->is_free()) {
            if (block->get_start() != current) {
                std::memmove(current, block->get_start(), block->get_size());
                block->set_start(current);
            }
            current += block->get_size();
        }
    }
    used_size_ = current - static_cast<char*>(memory_block_);
}

void MemoryManager::merge_adjacent_blocks() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (blocks_.size() <= 1) {
        return;
    }
    
    // Sort blocks by address
    std::vector<std::pair<uint64_t, MemoryBlock*>> sorted_blocks;
    for (auto& pair : blocks_) {
        sorted_blocks.push_back({pair.first, pair.second.get()});
    }
    
    std::sort(sorted_blocks.begin(), sorted_blocks.end(), 
             [](const auto& a, const auto& b) {
                 return a.second->get_start() < b.second->get_start();
             });
    
    for (size_t i = 0; i < sorted_blocks.size() - 1; i++) {
        auto& current_id = sorted_blocks[i].first;
        auto* current = sorted_blocks[i].second;
        auto* next = sorted_blocks[i + 1].second;
        
        // If both blocks are free and they are adjacent, merge them
        if (current->is_free() && next->is_free()) {
            // Check if blocks are adjacent
            char* current_end = static_cast<char*>(current->get_start()) + current->get_size();
            if (current_end == next->get_start()) {
                // Create a new merged block
                auto merged_block = std::make_unique<MemoryBlock>(
                    current->get_start(),
                    current->get_size() + next->get_size(),
                    true  // Free block
                );
                
                // Replace the current block with the merged one
                blocks_[current_id] = std::move(merged_block);
                
                // Remove the next block
                blocks_.erase(sorted_blocks[i + 1].first);
                
                // Restart the process as our sorted_blocks is now invalid
                merge_adjacent_blocks();
                return;
            }
        }
    }
}

void MemoryManager::log_memory_operation(const std::string& operation, uint64_t id, size_t size) {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << dump_folder_ << "/memory_log_"
       << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S")
       << "_" << std::setfill('0') << std::setw(3) << now_ms.count()
       << ".log";
    
    std::ofstream file(ss.str(), std::ios::app);
    if (file) {
        file << operation << " ID: " << id << " Size: " << size 
             << " Total Used: " << used_size_ << "/" << total_size_ << std::endl;
    }
} 