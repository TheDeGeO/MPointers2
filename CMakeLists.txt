cmake_minimum_required(VERSION 3.10)
project(MPointers2 VERSION 2.0 LANGUAGES CXX)

# Set policies
cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Protobuf 3.21 REQUIRED)
find_package(gRPC 1.50.0 CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Configure Abseil with version compatible with gRPC 1.50.0
pkg_check_modules(ABSL REQUIRED IMPORTED_TARGET absl_base=20220623 absl_synchronization=20220623 absl_strings=20220623 absl_status=20220623 absl_statusor=20220623)

# Configure RE2 with version compatible with gRPC 1.50.0
pkg_check_modules(RE2 REQUIRED IMPORTED_TARGET re2=10.0.0)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Set RPATH
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Link libraries
target_link_libraries(mem-mgr
    PRIVATE
    proto_lib
    mpointers
    memory_manager
    gRPC::grpc++
    gRPC::grpc++_unsecure
    gRPC::grpc
    gRPC::grpc_unsecure
    protobuf::libprotobuf
    PkgConfig::ABSL
    PkgConfig::RE2
    Threads::Threads
)