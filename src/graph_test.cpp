#include "graph.h"
#include <iostream>

bool assert(int actual, int expect) {
    return actual == expect;
}

int main() {
    Graph* G = new Graph();
    G->n = 6;
    G->m = 6;
    int edgeData[6][2] = {
        {0, 1},
        {0, 2},
        {1, 2},
        {3, 4},
        {3, 5},
        {4, 5},
    };
    for (int i = 0; i < 6; ++i) {
        if (G->edges.find(edgeData[i][0]) == G->edges.end()) {
            G->edges[edgeData[i][0]] = std::vector<int>();
        }
        G->edges[edgeData[i][0]].push_back(edgeData[i][1]);
    }
    if (!assert(G->decompose().size(), 2)) {
        std::cout << "class Graph: FAIL" << std::endl;
        return 0;
    }
    delete G;
    std::cout << "class Graph: OK" << std::endl;
    return 0;
}