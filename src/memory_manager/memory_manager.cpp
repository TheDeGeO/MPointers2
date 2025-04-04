#include "memory_manager.h"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <filesystem>

// Initialize static instance
MemoryManager* MemoryManager::instance = nullptr;

MemoryManager* MemoryManager::getInstance() {
    if (!instance) {
        instance = new MemoryManager();
    }
    return instance;
}

bool MemoryManager::initialize(uint16_t port, size_t memSize, const std::string& dumpFolder) {
    totalSize = memSize;
    memoryBlock = malloc(memSize);
    if (!memoryBlock) return false;
    
    dumpFolderPath = dumpFolder;
    std::filesystem::create_directories(dumpFolderPath);
    
    nextBlockId = 0;
    
    // Initialize GC
    gc = std::make_unique<GarbageCollector>(this);
    
    return true;
}

void MemoryManager::setServer(std::unique_ptr<grpc::Server> srv) {
    server = std::move(srv);
}

void MemoryManager::waitForServer() {
    if (server) {
        server->Wait();
    } else {
        std::cerr << "Error: Server is not initialized\n";
    }
}

void MemoryManager::start() {
    gc->start();
}

void MemoryManager::stop() {
    gc->stop();
    if (server) {
        server->Shutdown();
    }
}

MemoryManager::~MemoryManager() {
    stop();
    if (memoryBlock) {
        free(memoryBlock);
    }
}

uint32_t MemoryManager::createBlock(size_t size, const std::string& type) {
    std::lock_guard<std::mutex> lock(mutex);
    
    // Find first fit
    size_t currentOffset = 0;
    for (const auto& block : blocks) {
        if (!block.isUsed && block.size >= size) {
            // Found a suitable block
            MemoryBlock newBlock{
                nextBlockId++,
                size,
                currentOffset,
                type,
                1,  // Initial ref count
                true
            };
            blocks.push_back(newBlock);
            dumpMemoryState();
            return newBlock.id;
        }
        currentOffset += block.size;
    }
    
    // No suitable block found, create at end if space available
    if (currentOffset + size <= totalSize) {
        MemoryBlock newBlock{
            nextBlockId++,
            size,
            currentOffset,
            type,
            1,  // Initial ref count
            true
        };
        blocks.push_back(newBlock);
        dumpMemoryState();
        return newBlock.id;
    }
    
    // No space available
    return -1;
}

bool MemoryManager::setValue(uint32_t id, const void* value, size_t size) {
    std::lock_guard<std::mutex> lock(mutex);
    
    for (auto& block : blocks) {
        if (block.id == id && block.isUsed) {
            if (size > block.size) return false;
            char* dest = static_cast<char*>(memoryBlock) + block.offset;
            std::memcpy(dest, value, size);
            dumpMemoryState();
            return true;
        }
    }
    return false;
}

bool MemoryManager::getValue(uint32_t id, void* value, size_t size) {
    std::lock_guard<std::mutex> lock(mutex);
    
    for (const auto& block : blocks) {
        if (block.id == id && block.isUsed) {
            if (size > block.size) return false;
            const char* src = static_cast<const char*>(memoryBlock) + block.offset;
            std::memcpy(value, src, size);
            return true;
        }
    }
    return false;
}

bool MemoryManager::increaseRefCount(uint32_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    
    for (auto& block : blocks) {
        if (block.id == id && block.isUsed) {
            block.refCount++;
            dumpMemoryState();
            return true;
        }
    }
    return false;
}

bool MemoryManager::decreaseRefCount(uint32_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    
    for (auto& block : blocks) {
        if (block.id == id && block.isUsed) {
            if (block.refCount > 0) {
                block.refCount--;
                dumpMemoryState();
                return true;
            }
        }
    }
    return false;
}

void MemoryManager::defragment() {
    std::lock_guard<std::mutex> lock(mutex);
    
    // Sort blocks by offset
    std::sort(blocks.begin(), blocks.end(), 
              [](const MemoryBlock& a, const MemoryBlock& b) {
                  return a.offset < b.offset;
              });
    
    size_t newOffset = 0;
    for (auto& block : blocks) {
        if (block.isUsed) {
            if (block.offset != newOffset) {
                // Move memory
                char* src = static_cast<char*>(memoryBlock) + block.offset;
                char* dest = static_cast<char*>(memoryBlock) + newOffset;
                std::memmove(dest, src, block.size);
                block.offset = newOffset;
            }
            newOffset += block.size;
        }
    }
    
    dumpMemoryState();
}

void MemoryManager::dumpMemoryState() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    );
    
    std::stringstream filename;
    filename << dumpFolderPath << "/memory_dump_" 
            << std::fixed << std::setprecision(3)
            << (now_ms.count() / 1000.0) << ".txt";
            
    std::ofstream dump(filename.str());
    if (!dump) return;
    
    dump << "Memory State Dump\n";
    dump << "Total Size: " << totalSize << " bytes\n";
    dump << "Blocks:\n";
    
    for (const auto& block : blocks) {
        dump << "ID: " << block.id
             << ", Type: " << block.type
             << ", Size: " << block.size
             << ", Offset: " << block.offset
             << ", RefCount: " << block.refCount
             << ", Used: " << (block.isUsed ? "Yes" : "No")
             << "\n";
             
        if (block.isUsed) {
            dump << "Content (hex): ";
            const unsigned char* data = static_cast<const unsigned char*>(memoryBlock) + block.offset;
            for (size_t i = 0; i < block.size && i < 32; ++i) {
                dump << std::hex << std::setw(2) << std::setfill('0')
                     << static_cast<int>(data[i]) << " ";
            }
            if (block.size > 32) dump << "...";
            dump << "\n";
        }
        dump << "\n";
    }
}

// GRPC Service Implementation
grpc::Status MemoryManager::Create(grpc::ServerContext* context,
                                  const memory_service::CreateRequest* request,
                                  memory_service::CreateResponse* response) {
    // Convertir el enum DataType a string
    std::string type_str;
    switch (request->type()) {
        case memory_service::INT:
            type_str = "INT";
            break;
        case memory_service::FLOAT:
            type_str = "FLOAT";
            break;
        case memory_service::DOUBLE:
            type_str = "DOUBLE";
            break;
        case memory_service::CHAR:
            type_str = "CHAR";
            break;
        case memory_service::BOOL:
            type_str = "BOOL";
            break;
        case memory_service::CUSTOM:
            type_str = "CUSTOM";
            break;
        default:
            type_str = "UNKNOWN";
    }
    
    uint32_t id = createBlock(request->size(), type_str);
    
    response->set_success(id != -1);
    if (id != -1) {
        response->set_id(id);
    } else {
        response->set_error_message("Failed to allocate memory block");
    }
    
    return grpc::Status::OK;
}

grpc::Status MemoryManager::Set(grpc::ServerContext* context,
                               const memory_service::SetRequest* request,
                               memory_service::SetResponse* response) {
    bool success = setValue(request->id(),
                           request->value().data(),
                           request->value().size());
                               
    response->set_success(success);
    if (!success) {
        response->set_error_message("Failed to set value");
    }
    
    return grpc::Status::OK;
}

grpc::Status MemoryManager::Get(grpc::ServerContext* context,
                               const memory_service::GetRequest* request,
                               memory_service::GetResponse* response) {
    // Find block size first
    size_t size = 0;
    {
        std::lock_guard<std::mutex> lock(mutex);
        for (const auto& block : blocks) {
            if (block.id == request->id() && block.isUsed) {
                size = block.size;
                break;
            }
        }
    }
    
    if (size == 0) {
        response->set_success(false);
        response->set_error_message("Block not found");
        return grpc::Status::OK;
    }
    
    std::vector<char> buffer(size);
    bool success = getValue(request->id(), buffer.data(), size);
    
    response->set_success(success);
    if (success) {
        response->set_value(buffer.data(), size);
    } else {
        response->set_error_message("Failed to get value");
    }
    
    return grpc::Status::OK;
}

grpc::Status MemoryManager::IncreaseRefCount(grpc::ServerContext* context,
                                           const memory_service::RefCountRequest* request,
                                           memory_service::RefCountResponse* response) {
    bool success = increaseRefCount(request->id());
    
    response->set_success(success);
    if (!success) {
        response->set_error_message("Failed to increase reference count");
    }
    
    return grpc::Status::OK;
}

grpc::Status MemoryManager::DecreaseRefCount(grpc::ServerContext* context,
                                           const memory_service::RefCountRequest* request,
                                           memory_service::RefCountResponse* response) {
    bool success = decreaseRefCount(request->id());
    
    response->set_success(success);
    if (!success) {
        response->set_error_message("Failed to decrease reference count");
    }
    
    return grpc::Status::OK;
}

// GarbageCollector implementation
MemoryManager::GarbageCollector::GarbageCollector(MemoryManager* mgr)
    : manager(mgr), running(false), interval(std::chrono::milliseconds(1000)) {}

void MemoryManager::GarbageCollector::start() {
    running = true;
    thread = std::thread(&GarbageCollector::run, this);
}

void MemoryManager::GarbageCollector::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void MemoryManager::GarbageCollector::run() {
    while (running) {
        std::this_thread::sleep_for(interval);
        std::lock_guard<std::mutex> lock(manager->mutex);
        
        // Check for blocks with zero references
        for (auto it = manager->blocks.begin(); it != manager->blocks.end();) {
            if (it->isUsed && it->refCount == 0) {
                it->isUsed = false;
                manager->dumpMemoryState();
            }
            ++it;
        }
    }
}