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
    std::cout << "Testing linked list implementation..." << std::endl;
    
    // Create a linked list with 5 nodes
    MPointer<Node> head = MPointer<Node>::New();
    *head = Node{1, MPointer<Node>::New()};
    
    MPointer<Node> current = head->next;
    for (int i = 2; i <= 5; ++i) {
        *current = Node{i, MPointer<Node>::New()};
        current = current->next;
    }
    *current = Node{6, MPointer<Node>()};  // Last node with null next
    
    // Verify the list
    current = head;
    int expected = 1;
    while (current.is_valid()) {
        assert(current->data == expected++);
        current = current->next;
    }
    assert(expected == 7);  // Should have counted 1 through 6
    
    std::cout << "Linked list test passed!" << std::endl;
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

void test_performance() {
    std::cout << "Testing performance..." << std::endl;
    
    const int num_nodes = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    
    MPointer<Node> head = MPointer<Node>::New();
    *head = Node{1, MPointer<Node>::New()};
    
    MPointer<Node> current = head->next;
    for (int i = 2; i <= num_nodes; ++i) {
        *current = Node{i, MPointer<Node>::New()};
        current = current->next;
    }
    *current = Node{num_nodes + 1, MPointer<Node>()};
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Created " << num_nodes << " nodes in " << duration.count() << "ms" << std::endl;
    
    // Verify the list
    current = head;
    int count = 0;
    while (current.is_valid()) {
        assert(current->data == ++count);
        current = current->next;
    }
    assert(count == num_nodes + 1);
    
    std::cout << "Performance test passed!" << std::endl;
}

int main() {
    try {
        // Initialize MPointer with the Memory Manager server address
        MPointer<Node>::Init("localhost:50051");
        
        // Run all tests
        test_basic_operations();
        test_linked_list();
        test_memory_management();
        test_error_handling();
        test_performance();
        
        std::cout << "All tests passed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 