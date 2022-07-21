#ifndef GRAPH
#define GRAPH

#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

class Graph {
    public:
        int n, m;
        std::unordered_map<int, std::vector<int>> edges;
        std::unordered_map<int, int> coreNum;

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