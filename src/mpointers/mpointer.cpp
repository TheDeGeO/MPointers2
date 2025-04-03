#include "mpointer.h"
#include "node.h"
#include <grpcpp/grpcpp.h>
#include "memory_service.grpc.pb.h"
#include <cstring>

template<typename T>
std::unique_ptr<memory_service::MemoryManager::Stub> MPointer<T>::stub_;

template<typename T>
void MPointer<T>::Init(const std::string& server_address) {
    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    stub_ = memory_service::MemoryManager::NewStub(channel);
}

template<typename T>
MPointer<T>::MPointer() : id_(0) {}

template<typename T>
MPointer<T>::~MPointer() {
    if (id_ != 0) {
        decrease_ref_count();
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
    if (id_ != other.id_) {
        if (id_ != 0) {
            decrease_ref_count();
        }
        id_ = other.id_;
        if (id_ != 0) {
            increase_ref_count();
        }
    }
    return *this;
}

template<typename T>
MPointer<T>::MPointer(MPointer&& other) noexcept : id_(other.id_) {
    other.id_ = 0;
}

template<typename T>
MPointer<T>& MPointer<T>::operator=(MPointer&& other) noexcept {
    if (id_ != other.id_) {
        if (id_ != 0) {
            decrease_ref_count();
        }
        id_ = other.id_;
        other.id_ = 0;
    }
    return *this;
}

template<typename T>
T& MPointer<T>::operator*() {
    T* value = new T(get_value());
    return *value;
}

template<typename T>
const T& MPointer<T>::operator*() const {
    T* value = new T(get_value());
    return *value;
}

template<typename T>
T* MPointer<T>::operator->() {
    T* value = new T(get_value());
    return value;
}

template<typename T>
const T* MPointer<T>::operator->() const {
    T* value = new T(get_value());
    return value;
}

template<typename T>
MPointer<T> MPointer<T>::New() {
    MPointer<T> ptr;
    
    memory_service::CreateRequest request;
    request.set_size(1);
    
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
        throw std::runtime_error("Unsupported type for MPointer");
    }
    
    memory_service::CreateResponse response;
    grpc::ClientContext context;
    
    grpc::Status status = stub_->Create(&context, request, &response);
    if (!status.ok() || !response.success()) {
        throw std::runtime_error("Failed to create memory block");
    }
    
    ptr.id_ = response.id();
    return ptr;
}

template<typename T>
void MPointer<T>::increase_ref_count() {
    memory_service::RefCountRequest request;
    request.set_id(id_);
    
    memory_service::RefCountResponse response;
    grpc::ClientContext context;
    
    grpc::Status status = stub_->IncreaseRefCount(&context, request, &response);
    if (!status.ok() || !response.success()) {
        throw std::runtime_error("Failed to increase reference count");
    }
}

template<typename T>
void MPointer<T>::decrease_ref_count() {
    memory_service::RefCountRequest request;
    request.set_id(id_);
    
    memory_service::RefCountResponse response;
    grpc::ClientContext context;
    
    grpc::Status status = stub_->DecreaseRefCount(&context, request, &response);
    if (!status.ok() || !response.success()) {
        throw std::runtime_error("Failed to decrease reference count");
    }
}

template<typename T>
void MPointer<T>::set_value(const T& value) {
    memory_service::SetRequest request;
    request.set_id(id_);
    request.set_value(std::string(reinterpret_cast<const char*>(&value), sizeof(T)));
    
    memory_service::SetResponse response;
    grpc::ClientContext context;
    
    grpc::Status status = stub_->Set(&context, request, &response);
    if (!status.ok() || !response.success()) {
        throw std::runtime_error("Failed to set value");
    }
}

template<typename T>
T MPointer<T>::get_value() const {
    memory_service::GetRequest request;
    request.set_id(id_);
    
    memory_service::GetResponse response;
    grpc::ClientContext context;
    
    grpc::Status status = stub_->Get(&context, request, &response);
    if (!status.ok() || !response.success()) {
        throw std::runtime_error("Failed to get value");
    }
    
    T value;
    std::memcpy(&value, response.value().data(), sizeof(T));
    return value;
}

// Explicit template instantiations
template class MPointer<int>;
template class MPointer<float>;
template class MPointer<double>;
template class MPointer<char>;
template class MPointer<bool>;
template class MPointer<Node>; 