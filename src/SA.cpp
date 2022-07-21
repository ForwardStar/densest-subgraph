#include "SA.h"

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
    }

    edgeDensity = double(edgeNum) / currentVertices.size();
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
    }

    currentVertexSet.erase(vertex);
    if (currentVertices.size() == 0) {
        edgeDensity = 0;
    } else {
        edgeDensity = edgeNum / currentVertices.size();
    }
    return vertex;
}

void SA::process(int times, double decayRate, double initialTemp, double minTemp) {
    if (G->m == G->n - 1) {
        ans = double(G->m) / G->n;
        return;
    }
    if (G->m == G->n) {
        ans = 1;
        return;
    }
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

SA::SA(Graph* GInput) {
    srand(time(0));
    G = GInput;
    std::unordered_map<int, std::vector<int>>::iterator it;
    for (it = G->edges.begin(); it != G->edges.end(); it++) {
        remainingVertices.push_back(it->first);
    }
}