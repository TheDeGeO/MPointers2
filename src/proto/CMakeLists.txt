# Find protoc
find_program(PROTOC protoc)

# Find gRPC plugin
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin)

# Generate protobuf and gRPC files
add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/memory_service.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/memory_service.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}/memory_service.grpc.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/memory_service.grpc.pb.h
    COMMAND ${PROTOC}
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
        --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
        --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_service.proto
    DEPENDS memory_service.proto
)

# Create a library for the generated protobuf and gRPC files
add_library(proto_lib
    ${CMAKE_CURRENT_BINARY_DIR}/memory_service.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/memory_service.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/memory_service.grpc.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/memory_service.grpc.pb.h
)

target_link_libraries(proto_lib
    PUBLIC
    protobuf::libprotobuf
    gRPC::grpc++
)

# Make the generated headers available to other targets
target_include_directories(proto_lib
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Add the generated files to the source groups for better IDE organization
source_group("Generated" FILES ${CMAKE_CURRENT_BINARY_DIR}/memory_service.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/memory_service.pb.h ${CMAKE_CURRENT_BINARY_DIR}/memory_service.grpc.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/memory_service.grpc.pb.h) 