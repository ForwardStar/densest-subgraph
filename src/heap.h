#ifndef HEAP
#define HEAP

#include <vector>
#include <unordered_map>

class Heap {
    public:
        std::unordered_map<int, int> nodeMap;
        std::vector<std::pair<int, int>> heap;

        // Adjust the heap.
        void pushUp(int idx);

        // Adjust the heap.
        void pushDown(int idx);

        // Insert a (key, value) pair.
        void insert(int key, int value);

        // Pop the element with smallest value.
        int pop();

        // Check whether element is in the heap.
        bool in(int key);

        // The number of elements in the heap.
        int size();

        Heap() {}
        ~Heap() {}
};

#endif