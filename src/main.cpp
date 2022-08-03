#include <iostream>
#include <algorithm>
#include "graph.h"
#include "SA.h"

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
    for (it = CCs.begin(); it != CCs.end(); it++) {
        (*it)->shrink();
        SA* unit = new SA(*it);
        // Time complexity: O(a*n*log_b(d/c)).
        unit->process(3, 0.996, (*it)->n, 1e-12);
        if (optimalUnit == nullptr) {
            optimalUnit = unit;
        } else if (optimalUnit->ans < unit->ans) {
            delete optimalUnit;
            optimalUnit = unit;
        } else {
            delete unit;
        }
    }
    std::cout << "Maximum edge density: " << optimalUnit->ans << std::endl;
    optimalUnit->print();
    std::cout << "Time: " << difftime(time(NULL), start_time) << "s" << std::endl;
    delete optimalUnit;
    return 0;
}