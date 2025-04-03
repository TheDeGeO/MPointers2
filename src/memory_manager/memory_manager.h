#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <thread>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <grpcpp/grpcpp.h>
#include "memory_service.grpc.pb.h"

class MemoryManager : public memory_service::MemoryManager::Service {
public:
    // Singleton pattern
    static MemoryManager* getInstance();

    // Initialize memory manager
    bool initialize(uint16_t port, size_t memSize, const std::string& dumpFolder);

    // Server management
    void setServer(std::unique_ptr<grpc::Server> srv);
    void waitForServer();
    void start();
    void stop();

    // Memory block management
    uint32_t createBlock(size_t size, const std::string& type);
    bool setValue(uint32_t id, const void* value, size_t size);
    bool getValue(uint32_t id, void* value, size_t size);
    bool increaseRefCount(uint32_t id);
    bool decreaseRefCount(uint32_t id);
    void defragment();
    void dumpMemoryState();

    ~MemoryManager();

    // Block structure
    struct MemoryBlock {
        uint32_t id;
        size_t size;
        size_t offset;
        std::string type;
        uint32_t refCount;
        bool isUsed;
    };

    // Made public to allow GC and service impl access
    std::mutex mutex;
    std::vector<MemoryBlock> blocks;

private:
    MemoryManager() = default;
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    static MemoryManager* instance;

    void* memoryBlock = nullptr;
    size_t totalSize = 0;
    uint32_t nextBlockId = 0;
    std::string dumpFolderPath;
    
    std::unique_ptr<grpc::Server> server;
    
    // Forward declaration of GarbageCollector
    class GarbageCollector;
    std::unique_ptr<GarbageCollector> gc;

    // GRPC service implementation
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
};

// Garbage Collector
class MemoryManager::GarbageCollector {
public:
    GarbageCollector(MemoryManager* manager);
    void start();
    void stop();
    void run();

private:
    MemoryManager* manager;
    std::thread thread;
    bool running;
    std::chrono::milliseconds interval;
}; 