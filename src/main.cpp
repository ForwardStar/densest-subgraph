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
    double ans = 0;
    for (it = CCs.begin(); it != CCs.end(); it++) {
        SA* unit = new SA(*it);
        // Time complexity: O(a*n*log_b(d/c)).
        unit->process(2, 0.99, (*it)->n, 1e-12);
        ans = std::max(ans, unit->ans);
        delete unit;
    }
    std::cout << "Maximum edge density: " << ans << std::endl;
    std::cout << "Time: " << difftime(time(NULL), start_time) << "s" << std::endl;
    return 0;
}