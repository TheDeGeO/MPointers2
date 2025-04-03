# Add the linked list test executable
add_executable(linked_list_test
    linked_list_test.cpp
)

target_link_libraries(linked_list_test
    PRIVATE
    mpointers
    memory_manager
    proto_lib
    protobuf::libprotobuf
    gRPC::grpc++
) 