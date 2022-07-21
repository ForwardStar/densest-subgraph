#include <ctime>
#include <random>
#include <cmath>
#include <unordered_set>
#include "graph.h"

class SA {
    public:
        double ans = 0;
        Graph* G;
        int edgeNum = 0;
        double edgeDensity;
        std::unordered_set<int> optimalVertexSet;
        std::unordered_set<int> currentVertexSet;
        std::vector<int> currentVertices;
        std::vector<int> remainingVertices;

        // Insert a random vertex in current vertex set.
        int insertVertex();

        // Remove a random vertex in current vertex set.
        int removeVertex();

        // Process returns the maximum edge density of graph `G`.
        void process(int times, double decayRate, double initialTemp, double minTemp);

        SA(Graph* GInput);
        ~SA() {}
};