#include <iostream>
#include <string>
#include "../src/mpointers/mpointer.h"
#include "../src/mpointers/node.h"

int main() {
    try {
        // Initialize MPointer with the Memory Manager server address
        MPointer<int>::Init("localhost:50051");

        // Create a linked list with 5 nodes
        MPointer<Node> head = MPointer<Node>::New();
        *head = Node{1, MPointer<Node>::New()};

        MPointer<Node> current = head->next;
        for (int i = 2; i <= 5; ++i) {
            *current = Node{i, MPointer<Node>::New()};
            current = current->next;
        }
        *current = Node{6, MPointer<Node>()};  // Last node with null next

        // Traverse and print the linked list
        std::cout << "Linked list contents:" << std::endl;
        current = head;
        while (current.id() != 0) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;

        // Demonstrate reference counting
        {
            MPointer<Node> temp = head;  // Reference count increases
            std::cout << "Temporary reference created" << std::endl;
        }  // Reference count decreases when temp goes out of scope

        // The linked list should still be accessible
        std::cout << "Linked list after temporary reference:" << std::endl;
        current = head;
        while (current.id() != 0) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 