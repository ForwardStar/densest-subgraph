#include "SA.h"

void SA::print() {
    std::vector<int> ans;
    std::unordered_set<int>::iterator it;
    std::cout << "Densest subgraph:" << std::endl;
    for (it = optimalVertexSet.begin(); it != optimalVertexSet.end(); it++) {
        ans.push_back(*it);
    }
    for (it = fixedVertexSet.begin(); it != fixedVertexSet.end(); it++) {
        ans.push_back(*it);
    }
    std::sort(ans.begin(), ans.end());
    std::vector<int>::iterator it1;
    for (it1 = ans.begin(); it1 != ans.end(); it1++) {
        std::cout << *it1 << " ";
    }
    std::cout << std::endl;
}

int SA::insertVertex() {
    if (remainingVertices.size() == 0) {
        return -1;
    }
    int idx = rand() % remainingVertices.size();
    int vertex = remainingVertices[idx];
    currentVertexSet.insert(vertex);
    currentVertices.push_back(vertex);
    remainingVertices.erase(remainingVertices.begin() + idx);

    std::vector<int>::iterator it;
    for (it = G->edges[vertex].begin(); it != G->edges[vertex].end(); it++) {
        if (currentVertexSet.find(*it) != currentVertexSet.end()) {
            edgeNum++;
        }
        if (fixedVertexSet.find(*it) != fixedVertexSet.end()) {
            edgeNum++;
        }
    }

    edgeDensity = double(edgeNum) / (currentVertices.size() + fixedVertexSet.size());
    return vertex;
}

int SA::removeVertex() {
    if (currentVertices.size() == 0) {
        return -1;
    }
    int idx = rand() % currentVertices.size();
    int vertex = currentVertices[idx];
    currentVertices.erase(currentVertices.begin() + idx);
    remainingVertices.push_back(vertex);

    std::vector<int>::iterator it;
    for (it = G->edges[vertex].begin(); it != G->edges[vertex].end(); it++) {
        if (currentVertexSet.find(*it) != currentVertexSet.end()) {
            edgeNum--;
        }
        if (fixedVertexSet.find(*it) != fixedVertexSet.end()) {
            edgeNum--;
        }
    }

    currentVertexSet.erase(vertex);
    if (currentVertices.size() == 0 && fixedVertexSet.size() == 0) {
        edgeDensity = 0;
    } else {
        edgeDensity = double(edgeNum) / (currentVertices.size() + fixedVertexSet.size());
    }
    return vertex;
}

void SA::process(int times, double decayRate, double initialTemp, double minTemp) {
    // if (G->m == G->n - 1) {
    //     ans = double(G->m) / G->n;
    //     return;
    // }
    // if (G->m == G->n) {
    //     ans = 1;
    //     return;
    // }
    double temperature = initialTemp;
    if (times == 0) {
        return;
    }
    while (temperature > minTemp) {
        int decision = rand() % 2;
        int previousNum = edgeNum;
        double previousDensity = edgeDensity;
        int u;
        if (decision == 0) {
            u = insertVertex();
        } else {
            u = removeVertex();
        }
        if (edgeDensity > previousDensity) {
            if (edgeDensity > ans) {
                ans = edgeDensity;
                optimalVertexSet.clear();
                std::vector<int>::iterator it;
                for (it = currentVertices.begin(); it != currentVertices.end(); it++) {
                    optimalVertexSet.insert(*it);
                }
            }
        } else {
            if (exp((edgeDensity - previousDensity) / temperature) * RAND_MAX > rand()) {
                // Do nothing.
            } else if (u != -1) {
                // Revert.
                if (decision == 0) {
                    currentVertices.pop_back();
                    remainingVertices.push_back(u);
                    currentVertexSet.erase(u);
                } else {
                    currentVertices.push_back(u);
                    remainingVertices.pop_back();
                    currentVertexSet.insert(u);
                }
                edgeNum = previousNum;
                edgeDensity = previousDensity;
            }
        }
        temperature *= decayRate;
    }
    process(times-1, decayRate, initialTemp, minTemp);
}

void SA::kprocess(int k, int times, double decayRate, double initialTemp, double minTemp) {
    double temperature = initialTemp;
    if (times == 0) {
        return;
    }
    if (currentVertices.size() == 0) {
        for (int i = 0; i < k; ++i) {
            int u = insertVertex();
        }
    }
    if (edgeDensity > ans) {
        ans = edgeDensity;
        optimalVertexSet.clear();
        std::vector<int>::iterator it;
        for (it = currentVertices.begin(); it != currentVertices.end(); it++) {
            optimalVertexSet.insert(*it);
        }
    }
    if (k == 0 || remainingVertices.size() == 0) {
        return;
    }
    while (temperature > minTemp) {
        double previousEdgeNum = edgeNum;
        double previousDensity = edgeDensity;
        int idx1 = rand() % currentVertices.size();
        int idx2 = rand() % remainingVertices.size();
        int vertex1 = *(currentVertices.begin() + idx1);
        int vertex2 = *(remainingVertices.begin() + idx2);
        std::vector<int>::iterator it;
        for (it = G->edges[vertex1].begin(); it != G->edges[vertex1].end(); it++) {
            if (currentVertexSet.find(*it) != currentVertexSet.end()) {
                edgeNum--;
            }
            if (fixedVertexSet.find(*it) != fixedVertexSet.end()) {
                edgeNum--;
            }
        }
        currentVertexSet.erase(vertex1);

        for (it = G->edges[vertex2].begin(); it != G->edges[vertex2].end(); it++) {
            if (currentVertexSet.find(*it) != currentVertexSet.end()) {
                edgeNum++;
            }
            if (fixedVertexSet.find(*it) != fixedVertexSet.end()) {
                edgeNum++;
            }
        }
        currentVertexSet.insert(vertex2);
        int temp = *(currentVertices.begin() + idx1);
        *(currentVertices.begin() + idx1) = *(remainingVertices.begin() + idx2);
        *(remainingVertices.begin() + idx2) = temp;

        edgeDensity = double(edgeNum) / (k + fixedVertexSet.size());
        if (edgeDensity > previousDensity) {
            if (edgeDensity > ans) {
                ans = edgeDensity;
                optimalVertexSet.clear();
                std::vector<int>::iterator it;
                for (it = currentVertices.begin(); it != currentVertices.end(); it++) {
                    optimalVertexSet.insert(*it);
                }
            }
        } else {
            if (exp((edgeDensity - previousDensity) / temperature) * RAND_MAX > rand()) {
                // Do nothing.
            } else {
                currentVertexSet.erase(vertex2);
                currentVertexSet.insert(vertex1);
                int temp = *(currentVertices.begin() + idx1);
                *(currentVertices.begin() + idx1) = *(remainingVertices.begin() + idx2);
                *(remainingVertices.begin() + idx2) = temp;
                edgeNum = previousEdgeNum;
                edgeDensity = previousDensity;
            }
        }
        temperature *= decayRate;
    }
    kprocess(k, times-1, decayRate, initialTemp, minTemp);
}

SA::SA(Graph* GInput) {
    srand(time(0));
    G = GInput;
    std::unordered_set<int>::iterator it;
    for (it = G->fixedVertexSet.begin(); it != G->fixedVertexSet.end(); it++) {
        fixedVertexSet.insert(*it);
    }
    std::unordered_map<int, std::vector<int>>::iterator it1;
    for (it1 = G->edges.begin(); it1 != G->edges.end(); it1++) {
        if (fixedVertexSet.find(it1->first) == fixedVertexSet.end()) {
            remainingVertices.push_back(it1->first);
        }
    }
    for (it1 = G->edges.begin(); it1 != G->edges.end(); it1++) {
        if (fixedVertexSet.find(it1->first) == fixedVertexSet.end()) {
            continue;
        }
        std::vector<int>::iterator it2;
        for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            if (fixedVertexSet.find(*it2) != fixedVertexSet.end()) {
                edgeNum++;
            }
        }
    }
    edgeNum /= 2;
    if (fixedVertexSet.size() != 0) {
        edgeDensity = double(edgeNum) / fixedVertexSet.size();
    }
}