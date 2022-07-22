#include "graph.h"
#include <iostream>

bool assert(double actual, double expect) {
    return actual == expect;
}

bool assert(int actual, int expect) {
    return actual == expect;
}

int main() {
    Graph* G = new Graph();
    G->n = 6;
    G->m = 12;
    int edgeData[12][2] = {
        {0, 1},
        {0, 2},
        {1, 0},
        {1, 2},
        {2, 0},
        {2, 1},
        {3, 4},
        {3, 5},
        {4, 3},
        {4, 5},
        {5, 3},
        {5, 4},
    };
    for (int i = 0; i < 12; ++i) {
        if (G->edges.find(edgeData[i][0]) == G->edges.end()) {
            G->edges[edgeData[i][0]] = std::vector<int>();
        }
        G->edges[edgeData[i][0]].push_back(edgeData[i][1]);
    }
    if (!assert(G->decompose().size(), 2)) {
        std::cout << "class Graph: FAIL" << std::endl;
        return 0;
    }
    G->coreDecomposition();
    if (!assert(G->coreNum.size(), G->n)) {
        std::cout << "class Graph: FAIL" << std::endl;
        return 0;
    }
    if (!assert(G->edgeDensity, 1.0)) {
        std::cout << "class Graph: FAIL" << std::endl;
        return 0;
    }
    std::unordered_map<int, int>::iterator it;
    for (it = G->coreNum.begin(); it != G->coreNum.end(); it++) {
        if (!assert(it->second, 2)) {
            std::cout << "class Graph: FAIL" << std::endl;
            return 0;
        }
    }
    delete G;
    std::cout << "class Graph: OK" << std::endl;
    return 0;
}