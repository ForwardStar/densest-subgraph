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
    unit->process(3, 0.996, 3000, 1e-12);
    units.push_back(unit);
}

int main(int argc, char* argv[]) {
    int start_time = time(NULL);
    Graph* G;
    if (argc == 1) {
        G = new Graph("datasets/test.txt");
    } else if (argc == 2) {
        G = new Graph(argv[1]);
        G->coreDecomposition();
        std::cout << "Edge density by naive greedy: " << G->edgeDensity << std::endl;
        G->shrink();
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
        double greedyAns = 0;
        for (int i = 0; i < CCs.size(); ++i) {
            greedyAns = std::max(greedyAns, CCs[i]->edgeDensity);
        }
        std::cout << "Edge density by optimized greedy: " << greedyAns << std::endl;
        for (int i = 0; i < CCs.size(); ++i) {
            if (optimalUnit == nullptr || optimalUnit->edgeDensity < units[i]->edgeDensity) {
                optimalUnit = units[i];
            }
        }
        std::cout << "Maximum edge density: " << optimalUnit->ans << std::endl;
        optimalUnit->print();
        std::vector<SA*>::iterator it1;
        for (it1 = units.begin(); it1 != units.end(); it1++) {
            delete *it1;
        }
    } else {
        G = new Graph(argv[1]);
        std::ifstream fin(argv[2]);
        int u;
        std::unordered_set<int> fixedVertexSet;
        while (fin >> u) {
            fixedVertexSet.insert(u);
        }
        G->fixedVertexSet = fixedVertexSet;
        G->anchoredDensity();
        std::cout << "Edge density by revised greedy: " << G->edgeDensity << std::endl;
        G->shrinkAnchored();
        // SA* unit = new SA(G);
        // unit->process(5, 0.998, 10000, 1e-12);
        SA* unit = nullptr;
        for (int i = 0; i <= G->n - fixedVertexSet.size(); i++) {
            SA* unit1 = new SA(G);
            unit1->kprocess(i, i / 1000 + 1, 0.99, 1000, 1e-10);
            if (unit == nullptr || unit->ans < unit1->ans) {
                unit = unit1;
            }
        }
        std::cout << "Maximum edge density: " << unit->ans << std::endl;
        unit->print();
        delete unit;
    }
    std::cout << "Time: " << difftime(time(NULL), start_time) << "s" << std::endl;
    return 0;
}