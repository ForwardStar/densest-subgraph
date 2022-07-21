#ifndef HEAP
#define HEAP

#include <unordered_map>

class Node {
    public:
        int key, value;
        Node* leftChild;
        Node* rightChild;
        Node* parent;

        void pushUp();

        void pushDown();
};

class Heap {
    public:
        std::unordered_map<int, Node*> nodeMap;
        Node* rootNode;

        // Insert a (key, value) pair.
        void insert(int key, int value);

        // Pop the element with smallest value.
        int pop();

        Heap() {}
        ~Heap() {}
};

#endif