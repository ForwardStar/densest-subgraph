#include <iostream>
#include <algorithm>
#include <thread>
#include "graph.h"
#include "SA.h"

std::vector<SA*> units;

void process(Graph* G) {
    G->shrink();
    SA* unit = new SA(G);
    // Time complexity: O(a*n*log_b(d/c)).
    unit->process(3, 0.996, G->n, 1e-12);
    units.push_back(unit);
}

int main(int argc, char* argv[]) {
    int start_time = time(NULL);
    Graph* G;
    if (argc == 1) {
        G = new Graph("datasets/test.txt");
    } else {
        G = new Graph(argv[1]);
    }
    std::vector<Graph*> CCs = G->decompose();
    std::vector<Graph*>::iterator it;
    SA* optimalUnit = nullptr;
    std::thread tids[CCs.size()];
    for (int i = 0; i < CCs.size(); ++i) {
        tids[i] = std::thread(process, CCs[i]);
    }
    for (int i = 0; i < CCs.size(); ++i) {
        tids[i].join();
    }
    for (int i = 0; i < CCs.size(); ++i) {
        if (optimalUnit == nullptr || optimalUnit->edgeDensity < units[i]->edgeDensity) {
            optimalUnit = units[i];
        }
    }
    std::cout << "Maximum edge density: " << optimalUnit->ans << std::endl;
    optimalUnit->print();
    std::cout << "Time: " << difftime(time(NULL), start_time) << "s" << std::endl;
    return 0;
}