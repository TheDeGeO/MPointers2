#include "mpointer.h"
#include "node.h"
#include <grpcpp/grpcpp.h>
#include <sstream>
#include <cstring>

// Include the generated proto files directly
#include "memory_service.pb.h"
#include "memory_service.grpc.pb.h"

template<typename T>
std::unique_ptr<memory_service::MemoryManager::Stub> MPointer<T>::stub_;

template<typename T>
std::chrono::milliseconds MPointer<T>::timeout_ = std::chrono::seconds(5);

template<typename T>
std::mutex MPointer<T>::stub_mutex_;

template<typename T>
void MPointer<T>::Init(const std::string& server_address, std::chrono::milliseconds timeout) {
    std::lock_guard<std::mutex> lock(stub_mutex_);
    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    stub_ = memory_service::MemoryManager::NewStub(channel);
    timeout_ = timeout;
}

template<typename T>
MPointer<T>::MPointer() : id_(0) {}

template<typename T>
MPointer<T>::~MPointer() {
    if (id_ != 0) {
        try {
            decrease_ref_count();
        } catch (...) {
            // Ignore errors during destruction
        }
    }
}

template<typename T>
MPointer<T>::MPointer(const MPointer& other) : id_(other.id_) {
    if (id_ != 0) {
        increase_ref_count();
    }
}

template<typename T>
MPointer<T>& MPointer<T>::operator=(const MPointer& other) {
    // Skip self-assignment
    if (this == std::addressof(other)) {
        return *this;
    }
    if (id_ != 0) {
        decrease_ref_count();
    }
    id_ = other.id_;
    if (id_ != 0) {
        increase_ref_count();
    }
    return *this;
}

template<typename T>
MPointer<T>::MPointer(MPointer&& other) noexcept : id_(other.id_) {
    other.id_ = 0;
}

template<typename T>
MPointer<T>& MPointer<T>::operator=(MPointer&& other) noexcept {
    // Skip self-assignment
    if (this == std::addressof(other)) {
        return *this;
    }
    if (id_ != 0) {
        decrease_ref_count();
    }
    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

template<typename T>
MPointer<T>& MPointer<T>::operator=(const T& value) {
    check_connection();
    if (id_ == 0) {
        *this = New();
    }
    set_value(value);
    return *this;
}

template<typename T>
T& MPointer<T>::operator*() {
    check_connection();
    if (id_ == 0) {
        throw MPointerException("Cannot dereference null MPointer");
    }
    static T value = get_value();
    return value;
}

template<typename T>
const T& MPointer<T>::operator*() const {
    check_connection();
    if (id_ == 0) {
        throw MPointerException("Cannot dereference null MPointer");
    }
    static T value = get_value();
    return value;
}

template<typename T>
T* MPointer<T>::operator->() {
    return &operator*();
}

template<typename T>
const T* MPointer<T>::operator->() const {
    return &operator*();
}

template<typename T>
MPointer<T> MPointer<T>::New() {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    MPointer<T> ptr;
    memory_service::CreateRequest request;
    memory_service::CreateResponse response;
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + timeout_);
    
    // Set request parameters
    request.set_size(sizeof(T));
    
    // Determine the data type
    if (std::is_same<T, int>::value) {
        request.set_type(memory_service::INT);
    } else if (std::is_same<T, float>::value) {
        request.set_type(memory_service::FLOAT);
    } else if (std::is_same<T, double>::value) {
        request.set_type(memory_service::DOUBLE);
    } else if (std::is_same<T, char>::value) {
        request.set_type(memory_service::CHAR);
    } else if (std::is_same<T, bool>::value) {
        request.set_type(memory_service::BOOL);
    } else {
        request.set_type(memory_service::CUSTOM);
    }
    
    grpc::Status status = stub_->Create(&context, request, &response);
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in Create: " << status.error_message();
        throw MPointerException(ss.str());
    }
    
    if (!response.success()) {
        throw MPointerException("Failed to create memory block: " + response.error_message());
    }
    
    ptr.id_ = response.id();
    return ptr;
}

template<typename T>
bool MPointer<T>::is_valid() const {
    return id_ != 0;
}

template<typename T>
void MPointer<T>::reset() {
    if (id_ != 0) {
        decrease_ref_count();
        id_ = 0;
    }
}

template<typename T>
void MPointer<T>::increase_ref_count() {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    memory_service::RefCountRequest request;
    memory_service::RefCountResponse response;
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + timeout_);
    
    request.set_id(id_);
    grpc::Status status = stub_->IncreaseRefCount(&context, request, &response);
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in IncreaseRefCount: " << status.error_message();
        throw MPointerException(ss.str());
    }
    
    if (!response.success()) {
        throw MPointerException("Failed to increase reference count");
    }
}

template<typename T>
void MPointer<T>::decrease_ref_count() {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    memory_service::RefCountRequest request;
    memory_service::RefCountResponse response;
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + timeout_);
    
    request.set_id(id_);
    grpc::Status status = stub_->DecreaseRefCount(&context, request, &response);
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in DecreaseRefCount: " << status.error_message();
        throw MPointerException(ss.str());
    }
    
    if (!response.success()) {
        throw MPointerException("Failed to decrease reference count");
    }
}

template<typename T>
void MPointer<T>::set_value(const T& value) {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    memory_service::SetRequest request;
    memory_service::SetResponse response;
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + timeout_);
    
    request.set_id(id_);
    std::string value_str(reinterpret_cast<const char*>(&value), sizeof(T));
    request.set_value(value_str);
    
    grpc::Status status = stub_->Set(&context, request, &response);
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in Set: " << status.error_message();
        throw MPointerException(ss.str());
    }
    
    if (!response.success()) {
        throw MPointerException("Failed to set value: " + response.error_message());
    }
}

template<typename T>
T MPointer<T>::get_value() const {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    memory_service::GetRequest request;
    memory_service::GetResponse response;
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + timeout_);
    
    request.set_id(id_);
    grpc::Status status = stub_->Get(&context, request, &response);
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in Get: " << status.error_message();
        throw MPointerException(ss.str());
    }
    
    if (!response.success()) {
        throw MPointerException("Failed to get value: " + response.error_message());
    }
    
    if (response.value().size() != sizeof(T)) {
        throw MPointerException("Invalid value size");
    }
    
    T value;
    std::memcpy(&value, response.value().data(), sizeof(T));
    return value;
}

template<typename T>
void MPointer<T>::check_connection() const {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
}

template<typename T>
void MPointer<T>::handle_grpc_error(const grpc::Status& status, const std::string& operation) const {
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in " << operation << ": " << status.error_message();
        throw MPointerException(ss.str());
    }
}

// Explicit template instantiations
template class MPointer<int>;
template class MPointer<float>;
template class MPointer<double>;
template class MPointer<char>;
template class MPointer<bool>;
template class MPointer<Node>; 