# MPointers 2.0

A memory management system consisting of two main components: a Memory Manager service and an MPointers library. The Memory Manager administers a reserved memory block while the MPointers library enables applications to interact with this memory through a pointer-like interface.

## Features

- Memory Manager service that handles memory allocation and deallocation
- MPointers library that provides a pointer-like interface to managed memory
- Reference counting for automatic memory management
- Memory defragmentation
- gRPC-based communication between components
- Memory state dumps for debugging
- NodeStorage for persistent node data management

## Building the Project

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- gRPC and Protocol Buffers
- Thread support library

### Build Steps

1. Clone the repository:
```bash
git clone https://github.com/TheDeGeO/MPointers2
cd MPointers2
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure the project:
```bash
cmake ..
```

4. Build the project:
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

### Testing

The project includes several test programs:
- `linked_list_test`: Demonstrates MPointers with a linked list implementation
- `simple_test`: Basic MPointers functionality tests
- `grpc_test`: gRPC communication tests

## Memory Management

The Memory Manager uses a reference counting system for automatic memory management. When a memory block's reference count reaches zero, it is automatically freed by the garbage collector.

### NodeStorage

The system includes a NodeStorage component that provides persistent storage for node data, ensuring that node values are properly maintained across operations.
