# MPointers library
add_library(mpointers
    mpointer.h
    mpointer.cpp
    node.h
)

target_include_directories(mpointers
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../proto
)

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