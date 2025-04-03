# Add the linked list test executable
add_executable(linked_list_test
    linked_list_test.cpp
)

add_executable(simple_test
    simple_test.cpp
)

add_executable(grpc_test
    grpc_test.cpp
)

target_include_directories(linked_list_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src/proto
)

target_include_directories(simple_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src/proto
)

target_include_directories(grpc_test
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

target_link_libraries(simple_test
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

target_link_libraries(grpc_test
    PRIVATE
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

# Add dependencies to ensure proto files are generated first
add_dependencies(linked_list_test proto_lib)
add_dependencies(simple_test proto_lib)
add_dependencies(grpc_test proto_lib) 