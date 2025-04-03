# Add subdirectories
add_subdirectory(proto)
add_subdirectory(memory_manager)
add_subdirectory(mpointers)

# Link libraries to memory_manager
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

# Link libraries to mpointers
target_link_libraries(mpointers
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

# Add executable
add_executable(mem-mgr memory_manager/main.cpp)
target_link_libraries(mem-mgr
    PRIVATE
    memory_manager
    mpointers
    proto_lib
    gRPC::grpc++_unsecure
    protobuf::libprotobuf
    gRPC::grpc_unsecure
    gRPC::gpr
    PkgConfig::ABSL
    PkgConfig::RE2
    Threads::Threads
) 