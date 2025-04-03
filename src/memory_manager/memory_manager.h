#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "memory_service.grpc.pb.h"
#include "memory_block.h"
#include "garbage_collector.h"

class MemoryManager final : public memory_service::MemoryManager::Service {
public:
    MemoryManager(size_t memory_size, const std::string& dump_folder);
    ~MemoryManager();

    // gRPC service methods
    grpc::Status Create(grpc::ServerContext* context, 
                       const memory_service::CreateRequest* request,
                       memory_service::CreateResponse* response) override;

    grpc::Status Set(grpc::ServerContext* context,
                    const memory_service::SetRequest* request,
                    memory_service::SetResponse* response) override;

    grpc::Status Get(grpc::ServerContext* context,
                    const memory_service::GetRequest* request,
                    memory_service::GetResponse* response) override;

    grpc::Status IncreaseRefCount(grpc::ServerContext* context,
                                const memory_service::RefCountRequest* request,
                                memory_service::RefCountResponse* response) override;

    grpc::Status DecreaseRefCount(grpc::ServerContext* context,
                                const memory_service::RefCountRequest* request,
                                memory_service::RefCountResponse* response) override;

    // Memory management methods
    void* allocate(size_t size);
    void deallocate(uint64_t id);
    void defragment();

private:
    void* memory_block_;
    size_t total_size_;
    size_t used_size_;
    std::string dump_folder_;
    std::unordered_map<uint64_t, std::unique_ptr<MemoryBlock>> blocks_;
    std::mutex mutex_;
    std::unique_ptr<GarbageCollector> garbage_collector_;
    uint64_t next_id_;

    void generate_dump();
    bool is_valid_id(uint64_t id);
    size_t get_type_size(memory_service::DataType type);
}; 