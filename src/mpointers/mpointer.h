#pragma once

#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "memory_service.grpc.pb.h"

template<typename T>
class MPointer {
public:
    // Static initialization
    static void Init(const std::string& server_address);

    // Constructor and destructor
    MPointer();
    ~MPointer();

    // Copy constructor and assignment
    MPointer(const MPointer& other);
    MPointer& operator=(const MPointer& other);

    // Move constructor and assignment
    MPointer(MPointer&& other) noexcept;
    MPointer& operator=(MPointer&& other) noexcept;

    // Pointer operators
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;
    uint64_t operator&() const { return id_; }

    // Static factory method
    static MPointer<T> New();

    uint64_t id() const { return id_; }

private:
    uint64_t id_;
    static std::unique_ptr<memory_service::MemoryManager::Stub> stub_;

    // Helper methods
    void increase_ref_count();
    void decrease_ref_count();
    void set_value(const T& value);
    T get_value() const;
}; 