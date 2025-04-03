#pragma once

#include "mpointer.h"

struct Node {
    int data;
    MPointer<Node> next;
}; 