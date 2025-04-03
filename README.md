# MPointers 2.0

A memory management system consisting of two main components: a Memory Manager service and an MPointers library. The Memory Manager administers a reserved memory block while the MPointers library enables applications to interact with this memory through a pointer-like interface.

## Features

- Memory Manager service that handles memory allocation and deallocation
- MPointers library that provides a pointer-like interface to managed memory
- Reference counting for automatic memory management
- Memory defragmentation
- gRPC-based communication between components
- Memory state dumps for debugging

## Building the Project

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- gRPC and Protocol Buffers
- Thread support library

### Build Steps

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure the project:
```bash
cmake ..
```

3. Build the project:
```bash
make
```

## Running the Memory Manager

The Memory Manager can be started with the following command:
```bash
./mem-mgr --port LISTEN_PORT --memsize SIZE_MB --dumpFolder DUMP_FOLDER
```

Parameters:
- `LISTEN_PORT`: Port number for listening to gRPC requests
- `SIZE_MB`: Memory block size in megabytes to be managed
- `DUMP_FOLDER`: Directory path where memory state dumps will be stored

## Using MPointers

1. Initialize the MPointers library:
```cpp
MPointer<int>::Init("localhost:50051");  // Replace with actual server address
```

2. Create a new MPointer:
```cpp
MPointer<int> ptr = MPointer<int>::New();
```

3. Use the MPointer like a regular pointer:
```cpp
*ptr = 42;
int value = *ptr;
```

## Example: Linked List

The project includes a test that demonstrates the use of MPointers to implement a linked list. To run the test:

```bash
./linked_list_test
```

## Project Structure

- `src/memory_manager/`: Memory Manager service implementation
- `src/mpointers/`: MPointers library implementation
- `src/proto/`: Protocol Buffers definitions
- `tests/`: Test programs

## Design Patterns Used

- Singleton (for MPointer static initialization)
- Factory Method (for creating new MPointers)
- Observer (for garbage collection)
- Proxy (for the MPointer interface)

## Memory Management

The Memory Manager uses a reference counting system for automatic memory management. When a memory block's reference count reaches zero, it is automatically freed by the garbage collector.

## License

This project is licensed under the MIT License - see the LICENSE file for details. 