#ifndef NODE_H
#define NODE_H

#include <cstdint>
#include <vector>
#include <map>
#include <mutex>
#include <iostream>
#include <cstring>  // For std::memcpy

// Forward declaration
template<typename T>
class MPointer;

// Singleton class for node storage
class NodeStorage {
private:
    // Estructura para almacenar los datos del nodo
    struct NodeData {
        int data;
        uint64_t next_id;
    };

    // Map que relaciona ID del MPointer con los datos del nodo
    std::map<uint64_t, NodeData> nodes_;
    std::mutex mutex_;
    
    // Constructor privado para singleton
    NodeStorage() = default;
    
public:
    // Get singleton instance
    static NodeStorage& getInstance() {
        static NodeStorage instance;
        return instance;
    }
    
    // Clear all stored nodes
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        nodes_.clear();
        std::cout << "NodeStorage cleared" << std::endl;
    }
    
    // Store a node
    void store(uint64_t id, int data, uint64_t next_id) {
        std::lock_guard<std::mutex> lock(mutex_);
        nodes_[id] = {data, next_id};
        std::cout << "NodeStorage stored node " << id << ": data=" << data << ", next_id=" << next_id << std::endl;
    }
    
    // Retrieve a node
    bool retrieve(uint64_t id, int& data, uint64_t& next_id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = nodes_.find(id);
        if (it != nodes_.end()) {
            data = it->second.data;
            next_id = it->second.next_id;
            std::cout << "NodeStorage retrieved node " << id << ": data=" << data << ", next_id=" << next_id << std::endl;
            return true;
        }
        std::cout << "NodeStorage failed to retrieve node " << id << std::endl;
        return false;
    }
    
    // Debug helper
    void debugDump() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "--- NodeStorage Contents ---" << std::endl;
        if (nodes_.empty()) {
            std::cout << "  (empty)" << std::endl;
        } else {
            for (const auto& pair : nodes_) {
                std::cout << "  Node " << pair.first << ": data=" << pair.second.data << ", next_id=" << pair.second.next_id << std::endl;
            }
        }
        std::cout << "------------------------" << std::endl;
    }
};

// Simple node structure for linked list
struct Node {
    int data;
    uint64_t next_id;
    
    // Default constructor
    Node() : data(0), next_id(0) {}
    
    // Constructor with values
    Node(int d, uint64_t next) : data(d), next_id(next) {}
    
    // Serialization for sending over gRPC
    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> result(sizeof(int) + sizeof(uint64_t));
        
        // Copy data
        std::memcpy(result.data(), &data, sizeof(int));
        
        // Copy next_id
        std::memcpy(result.data() + sizeof(int), &next_id, sizeof(uint64_t));
        
        return result;
    }
    
    // Deserialization
    static Node deserialize(const std::vector<uint8_t>& bytes) {
        if (bytes.size() < sizeof(int) + sizeof(uint64_t)) {
            std::cerr << "Error: Insufficient data for Node deserialization" << std::endl;
            return Node(0, 0);
        }
        
        Node result;
        
        // Copy data
        std::memcpy(&result.data, bytes.data(), sizeof(int));
        
        // Copy next_id
        std::memcpy(&result.next_id, bytes.data() + sizeof(int), sizeof(uint64_t));
        
        return result;
    }
    
    // Friend method to set the MPointer ID directly (this avoids circular dependency)
    static void set_mpointer_id(MPointer<Node>& ptr, uint64_t id);
};

#endif // NODE_H 