cmake_minimum_required(VERSION 3.10)
project(MPointers2 VERSION 2.0 LANGUAGES CXX)

# Set policies
cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure we use system libraries
set(CMAKE_FIND_FRAMEWORK LAST)
set(CMAKE_FIND_APPBUNDLE LAST)

# Find required packages
find_package(Protobuf 25.1.0 REQUIRED CONFIG)
find_package(gRPC 1.60.0 CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Configure Abseil with version compatible with gRPC 1.60.0
pkg_check_modules(ABSL REQUIRED IMPORTED_TARGET
    absl_base=20230802
    absl_synchronization=20230802
    absl_strings=20230802
    absl_status=20230802
    absl_statusor=20230802
    absl_log_internal_message=20230802
    absl_log_internal_check_op=20230802
    absl_log_internal_conditions=20230802
    absl_log_internal_format=20230802
    absl_log_internal_globals=20230802
    absl_log_internal_log_sink_set=20230802
    absl_log_internal_nullguard=20230802
    absl_log_internal_proto=20230802
)

# Configure RE2 with version compatible with gRPC 1.60.0
pkg_check_modules(RE2 REQUIRED IMPORTED_TARGET re2=11.0.0)

# Add include directories for the entire project
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src/proto
)

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