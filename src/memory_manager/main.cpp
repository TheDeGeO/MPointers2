#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "memory_manager.h"

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
        // Create Memory Manager instance
        MemoryManager manager(memsize, dump_folder);

        // Create gRPC server
        grpc::ServerBuilder builder;
        builder.AddListeningPort("0.0.0.0:" + port, grpc::InsecureServerCredentials());
        builder.RegisterService(&manager);

        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Memory Manager server listening on port " << port << std::endl;
        std::cout << "Memory size: " << (memsize / (1024 * 1024)) << " MB" << std::endl;
        std::cout << "Dump folder: " << dump_folder << std::endl;

        // Wait for the server to shutdown
        server->Wait();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 