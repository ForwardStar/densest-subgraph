#include "heap.h"
#include <iostream>

bool assert(int actual, int expect) {
    return actual == expect;
}

int main() {
    Heap* heap = new Heap();
    heap->insert(1, 4);
    heap->insert(2, 3);
    if (!assert(heap->pop(), 2)) {
        std::cout << "class Heap: FAIL" << std::endl;
        return 0;
    }
    heap->insert(2, 3);
    heap->insert(3, 5);
    heap->insert(1, 6);
    if (!assert(heap->pop(), 2)) {
        std::cout << "class Heap: FAIL" << std::endl;
        return 0;
    }
    if (!assert(heap->pop(), 3)) {
        std::cout << "class Heap: FAIL" << std::endl;
        return 0;
    }
    if (!assert(heap->pop(), 1)) {
        std::cout << "class Heap: FAIL" << std::endl;
        return 0;
    }
    if (!assert(heap->pop(), -1)) {
        std::cout << "class Heap: FAIL" << std::endl;
        return 0;
    }
    delete heap;
    std::cout << "class Heap: OK" << std::endl;
    return 0;
}