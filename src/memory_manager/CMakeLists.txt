# Memory Manager library
add_library(memory_manager
    memory_manager.cpp
    memory_manager.h
    garbage_collector.cpp
    garbage_collector.h
    memory_block.cpp
    memory_block.h
)

target_include_directories(memory_manager
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../proto
)

target_link_libraries(memory_manager
    PRIVATE
    proto_lib
    gRPC::grpc++_unsecure
    protobuf::libprotobuf
    gRPC::grpc_unsecure
    gRPC::gpr
    PkgConfig::ABSL
    PkgConfig::RE2
    Threads::Threads
)

# Add dependency on proto_lib to ensure files are generated first
add_dependencies(memory_manager proto_lib) 