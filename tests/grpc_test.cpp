#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "memory_service.grpc.pb.h"

int main() {
    std::cout << "Creating channel to localhost:50051..." << std::endl;
    
    // Configurar opciones de canal con un timeout más largo
    grpc::ChannelArguments args;
    args.SetInt(GRPC_ARG_KEEPALIVE_TIME_MS, 10000);
    args.SetInt(GRPC_ARG_KEEPALIVE_TIMEOUT_MS, 10000);
    args.SetInt(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);
    
    auto channel = grpc::CreateCustomChannel(
        "localhost:50051",
        grpc::InsecureChannelCredentials(),
        args);
    
    std::cout << "Checking channel state..." << std::endl;
    auto state = channel->GetState(true);
    std::cout << "Initial state: " << state << std::endl;
    
    std::cout << "Attempting to connect..." << std::endl;
    for (int i = 0; i < 10; i++) {
        state = channel->GetState(true);
        std::cout << "State " << i << ": " << state << std::endl;
        
        // Intentar conectar con un timeout más largo
        if (channel->WaitForConnected(std::chrono::system_clock::now() + std::chrono::seconds(2))) {
            std::cout << "Successfully connected to server!" << std::endl;
            
            // Prueba simple RPC
            auto stub = memory_service::MemoryManager::NewStub(channel);
            grpc::ClientContext context;
            memory_service::CreateRequest request;
            memory_service::CreateResponse response;
            
            request.set_size(100);
            request.set_type(memory_service::INT);
            
            grpc::Status status = stub->Create(&context, request, &response);
            
            if (status.ok()) {
                std::cout << "RPC successful! Block ID: " << response.id() << std::endl;
            } else {
                std::cout << "RPC failed: " << status.error_message() << std::endl;
            }
            
            return 0;
        }
        
        std::cout << "Waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cerr << "Failed to connect to server after 10 attempts" << std::endl;
    return 1;
} 