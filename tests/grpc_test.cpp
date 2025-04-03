#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "memory_service.grpc.pb.h"

int main() {
    std::cout << "Creating channel to 127.0.0.1:50051..." << std::endl;
    auto channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
    
    std::cout << "Checking channel state..." << std::endl;
    auto state = channel->GetState(true);
    std::cout << "Initial state: " << state << std::endl;
    
    std::cout << "Attempting to connect..." << std::endl;
    for (int i = 0; i < 5; i++) {
        state = channel->GetState(true);
        std::cout << "State " << i << ": " << state << std::endl;
        
        if (state == GRPC_CHANNEL_READY) {
            std::cout << "Successfully connected to server!" << std::endl;
            return 0;
        }
        
        std::cout << "Waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cerr << "Failed to connect to server after 5 attempts" << std::endl;
    return 1;
} 