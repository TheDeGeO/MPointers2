#pragma once

#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "memory_service.pb.h"
#include "memory_service.grpc.pb.h"
#include <stdexcept>
#include <chrono>
#include <mutex>
#include <type_traits>

// Forward declaration
struct Node;

// Custom exception class for MPointer errors
class MPointerException : public std::runtime_error {
public:
    explicit MPointerException(const std::string& message) 
        : std::runtime_error(message) {}
};

template<typename T>
class MPointer {
public:
    // Static initialization with timeout
    static void Init(const std::string& server_address, 
                    std::chrono::milliseconds timeout = std::chrono::seconds(5));

    // Constructor and destructor
    MPointer();
    ~MPointer();

    // Copy constructor and assignment
    MPointer(const MPointer& other);
    MPointer& operator=(const MPointer& other);

    // Move constructor and assignment
    MPointer(MPointer&& other) noexcept;
    MPointer& operator=(MPointer&& other) noexcept;

    // Value assignment with error handling
    MPointer& operator=(const T& value);

    // Pointer operators with error handling
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;
    uint64_t operator&() const { return id_; }

    // Static factory method with error handling
    static MPointer<T> New();

    // Utility methods
    uint64_t id() const { return id_; }
    bool is_valid() const;
    void reset();
    
    // Función especial para deserialización
    void set_id_directly(uint64_t id) { id_ = id; }
    
    // Hacer amigo a la clase Node para que pueda acceder a id_
    friend struct Node;

private:
    uint64_t id_;
    static std::unique_ptr<memory_service::MemoryManager::Stub> stub_;
    static std::chrono::milliseconds timeout_;
    static std::mutex stub_mutex_;

    // Helper methods with error handling
    void increase_ref_count();
    void decrease_ref_count();
    void set_value(const T& value);
    T get_value() const;
    void check_connection() const;
    void handle_grpc_error(const grpc::Status& status, const std::string& operation) const;
}; 