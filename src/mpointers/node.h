#pragma once

template<typename T>
class MPointer;

struct Node {
    int data;
    MPointer<Node> next;
}; 