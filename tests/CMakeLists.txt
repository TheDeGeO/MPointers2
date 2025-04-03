# Add the linked list test executable
add_executable(linked_list_test
    linked_list_test.cpp
)

target_include_directories(linked_list_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src/proto
)

target_link_libraries(linked_list_test
    PRIVATE
    mpointers
    memory_manager
    proto_lib
    protobuf::libprotobuf
    gRPC::grpc++
    gRPC::grpc++_unsecure
    gRPC::grpc_unsecure
    gRPC::gpr
    PkgConfig::ABSL
    PkgConfig::RE2
    Threads::Threads
) 