#ifndef GRAPH
#define GRAPH

#include "heap.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class Graph {
    public:
        int n, m;
        int maxCoreNum = 0;
        double edgeDensity;
        std::unordered_set<int> fixedVertexSet;
        std::unordered_map<int, std::vector<int>> edges;
        std::unordered_map<int, int> coreNum;
        std::unordered_map<int, int> degree;
        // Compute an approximated anchored density.
        void anchoredDensity();

        // Compute core numbers of all vertices.
        void coreDecomposition();

        // Prune vertices definitely not in anchored densest subgraph; called after anchoredDensity().
        void shrinkAnchored();

        // Prune vertices definitely not in densest subgraphs.
        void shrink();

        // Decompose the graph into several connected components.
        std::vector<Graph*> decompose();

        // Build a graph from the given input file.
        // Graph file format:
        // ```
        // n
        // 0 u1 u2 ... ui
        // 1 v1 v2 ... vj
        // ...
        // n-1 t1 t2 ... tk
        // ```
        // where `n` in the first line represents the number of vertices;
        // `u1`, `u2`, ... represent edges from `0` -> `u1`, `u2`, ...;
        // totally `n` lines of edges are expected.
        Graph(char* graph_file);
        Graph() {}
        ~Graph() {}
};

#endif