#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include "../src/mpointers/mpointer.h"

int main() {
    try {
        std::cout << "Initializing MPointer with server at localhost:50051..." << std::endl;
        MPointer<int>::Init("localhost:50051", std::chrono::seconds(30));
        
        std::cout << "Creating new MPointer..." << std::endl;
        MPointer<int> ptr = MPointer<int>::New();
        
        std::cout << "Setting value..." << std::endl;
        *ptr = 42;
        
        std::cout << "Reading value: " << *ptr << std::endl;
        
        std::cout << "Test completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 