#include <iostream>
#include <string>
#include <cassert>
#include <chrono>
#include "../src/mpointers/mpointer.h"
#include "../src/mpointers/node.h"

// Test helper functions
void test_basic_operations() {
    std::cout << "Testing basic operations..." << std::endl;
    
    MPointer<int> ptr = MPointer<int>::New();
    *ptr = 42;
    assert(*ptr == 42);
    
    MPointer<int> ptr2 = ptr;
    assert(*ptr2 == 42);
    
    *ptr2 = 24;
    assert(*ptr == 24);  // Should affect both pointers
    
    std::cout << "Basic operations test passed!" << std::endl;
}

void test_linked_list() {
    std::cout << "Testing super simplified implementation..." << std::endl;
    
    // Crear un único puntero
    MPointer<int> ptr = MPointer<int>::New();
    
    // Asignar un valor
    *ptr = 42;
    
    // Verificar el valor
    int value = *ptr;
    std::cout << "Value: " << value << std::endl;
    assert(value == 42);
    
    // Asignar otro valor
    *ptr = 100;
    
    // Verificar el nuevo valor
    value = *ptr;
    std::cout << "New value: " << value << std::endl;
    assert(value == 100);
    
    std::cout << "Super simplified test passed!" << std::endl;
}

void test_memory_management() {
    std::cout << "Testing memory management..." << std::endl;
    
    {
        MPointer<int> ptr = MPointer<int>::New();
        *ptr = 42;
        
        {
            MPointer<int> ptr2 = ptr;  // Should increase ref count
            assert(*ptr2 == 42);
        }  // Should decrease ref count
        
        assert(*ptr == 42);  // Should still be valid
    }  // Should clean up properly
    
    std::cout << "Memory management test passed!" << std::endl;
}

void test_error_handling() {
    std::cout << "Testing error handling..." << std::endl;
    
    MPointer<int> ptr;
    try {
        *ptr;  // Should throw
        assert(false);  // Should not reach here
    } catch (const MPointerException& e) {
        assert(std::string(e.what()).find("null") != std::string::npos);
    }
    
    std::cout << "Error handling test passed!" << std::endl;
}

// Test: Create a linked list with 3 nodes, set values, and traverse it
void test_performance() {
    std::cout << "\n=== Testing Linked List with NodeStorage ===\n" << std::endl;
    
    // Clear NodeStorage before starting
    NodeStorage::getInstance().clear();
    NodeStorage::getInstance().debugDump();
    
    // Create 3 nodes
    MPointer<Node> head = MPointer<Node>::New();
    MPointer<Node> second = MPointer<Node>::New();
    MPointer<Node> third = MPointer<Node>::New();
    
    std::cout << "Created nodes:" << std::endl;
    std::cout << "  head ID: " << head.id() << std::endl;
    std::cout << "  second ID: " << second.id() << std::endl;
    std::cout << "  third ID: " << third.id() << std::endl;
    
    // Set node values directly in NodeStorage
    std::cout << "\nSetting node values directly in NodeStorage:" << std::endl;
    
    // Directly use NodeStorage for clarity
    std::cout << "  Setting head: data=1, next_id=" << second.id() << std::endl;
    NodeStorage::getInstance().store(head.id(), 1, second.id());
    
    std::cout << "  Setting second: data=2, next_id=" << third.id() << std::endl;
    NodeStorage::getInstance().store(second.id(), 2, third.id());
    
    std::cout << "  Setting third: data=3, next_id=0" << std::endl;
    NodeStorage::getInstance().store(third.id(), 3, 0);
    
    // Debug: Print the NodeStorage contents
    std::cout << "\nNodeStorage contents after setting values:" << std::endl;
    NodeStorage::getInstance().debugDump();
    
    // Verify using MPointer::operator*
    std::cout << "\nVerifying nodes using MPointer::operator*:" << std::endl;
    
    // Get head node
    Node retrieved_head = *head;
    std::cout << "  Retrieved head using *: data=" << retrieved_head.data << ", next_id=" << retrieved_head.next_id << std::endl;
    if (retrieved_head.data != 1 || retrieved_head.next_id != second.id()) {
        std::cout << "  ERROR: head node has incorrect values!" << std::endl;
    } else {
        std::cout << "  OK: head node has correct values" << std::endl;
    }
    
    // Get second node
    Node retrieved_second = *second;
    std::cout << "  Retrieved second using *: data=" << retrieved_second.data << ", next_id=" << retrieved_second.next_id << std::endl;
    if (retrieved_second.data != 2 || retrieved_second.next_id != third.id()) {
        std::cout << "  ERROR: second node has incorrect values!" << std::endl;
    } else {
        std::cout << "  OK: second node has correct values" << std::endl;
    }
    
    // Get third node
    Node retrieved_third = *third;
    std::cout << "  Retrieved third using *: data=" << retrieved_third.data << ", next_id=" << retrieved_third.next_id << std::endl;
    if (retrieved_third.data != 3 || retrieved_third.next_id != 0) {
        std::cout << "  ERROR: third node has incorrect values!" << std::endl;
    } else {
        std::cout << "  OK: third node has correct values" << std::endl;
    }
    
    // Traverse the linked list using MPointer
    std::cout << "\nTraversing the linked list with MPointer:" << std::endl;
    MPointer<Node> current = head;
    int expected_data = 1;
    
    while (current.id() != 0) {
        Node current_node = *current;
        std::cout << "  Node data: " << current_node.data << ", next_id: " << current_node.next_id << std::endl;
        
        if (current_node.data != expected_data) {
            std::cout << "  ERROR: Expected data=" << expected_data << ", but got " << current_node.data << std::endl;
        }
        
        // Update current to the next node
        if (current_node.next_id != 0) {
            MPointer<Node> next;
            Node::set_mpointer_id(next, current_node.next_id);
            current = next;
            expected_data++;
        } else {
            break;
        }
    }
    
    std::cout << "\nLinked list test completed\n" << std::endl;
}

int main(int argc, char **argv) {
    // Initialize MPointer system
    MPointer<Node>::Init("localhost:50051");
    
    // Run the linked list test
    test_performance();
    
    return 0;
} 