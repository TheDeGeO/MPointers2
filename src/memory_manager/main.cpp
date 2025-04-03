#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "memory_manager.h"
#include "memory_service.grpc.pb.h"

void print_usage() {
    std::cout << "Usage: ./mem-mgr --port LISTEN_PORT --memsize SIZE_MB --dumpFolder DUMP_FOLDER" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        print_usage();
        return 1;
    }

    std::string port;
    size_t memsize = 0;
    std::string dump_folder;

    // Parse command line arguments
    for (int i = 1; i < argc; i += 2) {
        std::string arg = argv[i];
        if (arg == "--port") {
            port = argv[i + 1];
        } else if (arg == "--memsize") {
            memsize = std::stoull(argv[i + 1]) * 1024 * 1024; // Convert MB to bytes
        } else if (arg == "--dumpFolder") {
            dump_folder = argv[i + 1];
        } else {
            print_usage();
            return 1;
        }
    }

    if (port.empty() || memsize == 0 || dump_folder.empty()) {
        print_usage();
        return 1;
    }

    try {
        std::cout << "Creating Memory Manager instance..." << std::endl;
        // Get Memory Manager instance
        auto manager = MemoryManager::getInstance();
        
        // Initialize memory manager
        if (!manager->initialize(std::stoi(port), memsize, dump_folder)) {
            std::cerr << "Failed to initialize memory manager" << std::endl;
            return 1;
        }

        std::cout << "Setting up gRPC server..." << std::endl;
        // Create gRPC server
        grpc::ServerBuilder builder;
        
        // Configure server options
        builder.SetMaxReceiveMessageSize(-1);
        builder.SetMaxSendMessageSize(-1);
        
        // Bind to localhost interface instead of all interfaces
        std::string server_address = "localhost:" + port;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(manager);

        std::cout << "Starting server..." << std::endl;
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        if (!server) {
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
        
        // Store server in memory manager
        manager->setServer(std::move(server));
        
        std::cout << "Memory Manager server listening on " << server_address << std::endl;
        std::cout << "Memory size: " << (memsize / (1024 * 1024)) << " MB" << std::endl;
        std::cout << "Dump folder: " << dump_folder << std::endl;

        // Start garbage collector
        manager->start();
        
        // Wait for the server to shutdown
        manager->waitForServer();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 