#include "mpointer.h"
#include "node.h"
#include <grpcpp/grpcpp.h>
#include <sstream>
#include <cstring>

// Include the generated proto files with correct paths
#include "../../build/src/proto/memory_service.pb.h"
#include "../../build/src/proto/memory_service.grpc.pb.h"

// Forward declare the specialization before any use
template<>
void MPointer<Node>::set_id_directly(uint64_t id);

// Implementación de la función auxiliar set_mpointer_id para Node
void Node::set_mpointer_id(MPointer<Node>& ptr, uint64_t id) {
    // Acceder directamente al campo privado id_ a través de un método amigo
    ptr.set_id_directly(id);
}

// Método auxiliar para set_id_directly en MPointer<Node>
template<>
void MPointer<Node>::set_id_directly(uint64_t id) {
    id_ = id;
}

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

// Especialización para Node
template<>
void MPointer<Node>::set_value(const Node& value) {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    // PUNTO CRÍTICO: guardar en NodeStorage
    std::cout << "NODE SET_VALUE - STORING IN NODESTORAGE: Node " << id_ << ": data=" << value.data 
              << ", next_id=" << value.next_id << std::endl;
    NodeStorage::getInstance().store(id_, value.data, value.next_id);
    
    // Ahora enviar al servidor remoto
    memory_service::SetRequest request;
    memory_service::SetResponse response;
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + timeout_);
    
    request.set_id(id_);
    
    // Serializar para el almacenamiento remoto - ya no usamos este dato
    std::vector<uint8_t> serialized = value.serialize();
    std::string data(reinterpret_cast<char*>(serialized.data()), serialized.size());
    request.set_value(data);
    
    grpc::Status status = stub_->Set(&context, request, &response);
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in Set: " << status.error_message();
        throw MPointerException(ss.str());
    }
    
    if (!response.success()) {
        throw MPointerException("Failed to set data: " + response.error_message());
    }
    
    std::cout << "Node value set successfully, ID: " << id_ << std::endl;
}

// Especialización para Node
template<>
Node MPointer<Node>::get_value() const {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    // PUNTO CRÍTICO: siempre obtener de NodeStorage
    int data = 0;
    uint64_t next_id = 0;
    if (NodeStorage::getInstance().retrieve(id_, data, next_id)) {
        std::cout << "NODE GET_VALUE - RETRIEVED FROM NODESTORAGE: Node " << id_ << ": data=" << data 
                  << ", next_id=" << next_id << std::endl;
        return Node(data, next_id);
    }
    
    // Si no está en caché local, crear un nodo vacío y advertir
    std::cerr << "NODE GET_VALUE - ERROR: Node " << id_ << " not found in NodeStorage! Returning empty node." << std::endl;
    return Node(0, 0);
}

// Especialización de New para Node
template<>
MPointer<Node> MPointer<Node>::New() {
    if (!stub_) {
        throw MPointerException("MPointer not initialized. Call Init() first.");
    }
    
    MPointer<Node> ptr;
    memory_service::CreateRequest request;
    memory_service::CreateResponse response;
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + timeout_);
    
    // Set request parameters - tamaño exacto para la estructura Node
    request.set_size(sizeof(int) + sizeof(uint64_t));  // data + next_id
    request.set_type(memory_service::CUSTOM);
    
    // Llamar al servicio de creación
    grpc::Status status = stub_->Create(&context, request, &response);
    if (!status.ok()) {
        std::stringstream ss;
        ss << "gRPC error in Create for Node: " << status.error_message();
        throw MPointerException(ss.str());
    }
    
    if (!response.success()) {
        throw MPointerException("Failed to create memory block for Node: " + response.error_message());
    }
    
    ptr.id_ = response.id();
    
    // Inicializar el nodo con valores por defecto explícitos
    Node empty_node(0, 0);  // data=0, next_id=0
    
    // Serializar e inicializar inmediatamente el nodo
    ptr.set_value(empty_node);
    
    std::cout << "Created new Node with ID: " << ptr.id_ << std::endl;
    
    return ptr;
}

// Add a specialization for operator= for MPointer<Node>
template<>
MPointer<Node>& MPointer<Node>::operator=(const Node& value) {
    check_connection();
    if (id_ == 0) {
        *this = New();
    }
    std::cout << "NODE OPERATOR= - Setting node " << id_ << " to data=" << value.data 
              << ", next_id=" << value.next_id << std::endl;
    
    // Use NodeStorage directly
    NodeStorage::getInstance().store(id_, value.data, value.next_id);
    
    // Also call set_value for remote storage
    set_value(value);
    
    return *this;
}

// Especialización del operador * para Node
template<>
Node& MPointer<Node>::operator*() {
    check_connection();
    if (id_ == 0) {
        throw MPointerException("Cannot dereference null MPointer");
    }
    
    // Crear un nodo estático que actualiza su valor desde NodeStorage
    static Node value;
    int data;
    uint64_t next_id;
    
    // Obtener los datos desde NodeStorage
    if (NodeStorage::getInstance().retrieve(id_, data, next_id)) {
        value.data = data;
        value.next_id = next_id;
        std::cout << "NODE OPERATOR* - Retrieved from NodeStorage: Node " << id_ 
                  << ": data=" << data << ", next_id=" << next_id << std::endl;
    } else {
        std::cerr << "NODE OPERATOR* - WARNING: Node " << id_ << " not found in NodeStorage!" << std::endl;
        // Devolver un nodo vacío si no se encuentra en NodeStorage
        value.data = 0;
        value.next_id = 0;
    }
    
    return value;
}

// Especialización del operador * const para Node
template<>
const Node& MPointer<Node>::operator*() const {
    check_connection();
    if (id_ == 0) {
        throw MPointerException("Cannot dereference null MPointer");
    }
    
    // Crear un nodo estático que actualiza su valor desde NodeStorage
    static Node value;
    int data;
    uint64_t next_id;
    
    // Obtener los datos desde NodeStorage
    if (NodeStorage::getInstance().retrieve(id_, data, next_id)) {
        value.data = data;
        value.next_id = next_id;
        std::cout << "NODE OPERATOR* CONST - Retrieved from NodeStorage: Node " << id_ 
                  << ": data=" << data << ", next_id=" << next_id << std::endl;
    } else {
        std::cerr << "NODE OPERATOR* CONST - WARNING: Node " << id_ << " not found in NodeStorage!" << std::endl;
        // Devolver un nodo vacío si no se encuentra en NodeStorage
        value.data = 0;
        value.next_id = 0;
    }
    
    return value;
}

// Explicit template instantiations
template class MPointer<int>;
template class MPointer<float>;
template class MPointer<double>;
template class MPointer<char>;
template class MPointer<bool>;
template class MPointer<Node>; 