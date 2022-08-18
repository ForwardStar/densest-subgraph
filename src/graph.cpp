#include "graph.h"

int find(std::unordered_map<int, int>* parent, int u) {
    if ((*parent)[u] != u) {
        (*parent)[u] = find(parent, (*parent)[u]);
    }
    return (*parent)[u];
}

void unioN(std::unordered_map<int, int>* parent, int u, int v) {
    if (parent->find(u) == parent->end()) {
        (*parent)[u] = u;
    }
    if (parent->find(v) == parent->end()) {
        (*parent)[v] = v;
    }
    u = find(parent, u);
    v = find(parent, v);
    if (u != v) {
        (*parent)[u] = v;
    }
}

void Graph::anchoredDensity() {
    int currentEdgeNum = 0;
    double currentEdgeDensity = 0;
    Heap* heap = new Heap();
    std::unordered_map<int, std::vector<int>>::iterator it;
    for (it = edges.begin(); it != edges.end(); it++) {
        degree[it->first] = it->second.size();
        currentEdgeNum += degree[it->first];
        if (fixedVertexSet.find(it->first) == fixedVertexSet.end()) {
            heap->insert(it->first, degree[it->first]);
        }
    }
    currentEdgeNum /= 2;
    currentEdgeDensity = double(currentEdgeNum) / n;
    edgeDensity = currentEdgeDensity;
    int vertex = heap->pop();
    while (vertex != -1) {
        std::vector<int>::iterator it1;
        for (it1 = edges[vertex].begin(); it1 != edges[vertex].end(); it1++) {
            if (fixedVertexSet.find(*it1) == fixedVertexSet.end() || heap->in(*it1)) {
                currentEdgeNum--;
                degree[*it1]--;
                if (heap->in(*it1)) {
                    heap->insert(*it1, degree[*it1]);
                }
            }
        }
        currentEdgeDensity = double(currentEdgeNum) / (heap->size() + fixedVertexSet.size());
        edgeDensity = std::max(currentEdgeDensity, edgeDensity);
        vertex = heap->pop();
    }
}

void Graph::coreDecomposition() {
    int currentEdgeNum = 0;
    double currentEdgeDensity = 0;
    Heap* heap = new Heap();
    std::unordered_map<int, std::vector<int>>::iterator it;
    for (it = edges.begin(); it != edges.end(); it++) {
        degree[it->first] = it->second.size();
        currentEdgeNum += degree[it->first];
        heap->insert(it->first, degree[it->first]);
    }
    currentEdgeNum /= 2;
    currentEdgeDensity = double(currentEdgeNum) / n;
    edgeDensity = currentEdgeDensity;
    maxCoreNum = 0;
    int vertex = heap->pop();
    while (vertex != -1) {
        int currentCoreNum = degree[vertex];
        maxCoreNum = std::max(maxCoreNum, currentCoreNum);
        coreNum[vertex] = maxCoreNum;
        std::vector<int>::iterator it1;
        for (it1 = edges[vertex].begin(); it1 != edges[vertex].end(); it1++) {
            if (heap->in(*it1)) {
                currentEdgeNum--;
                degree[*it1]--;
                heap->insert(*it1, degree[*it1]);
            }
        }
        currentEdgeDensity = double(currentEdgeNum) / heap->size();
        edgeDensity = std::max(currentEdgeDensity, edgeDensity);
        vertex = heap->pop();
    }
}

void Graph::shrinkAnchored() {
    std::unordered_map<int, std::vector<int>>::iterator it;
    for (it = edges.begin(); it != edges.end(); it++) {
        degree[it->first] = it->second.size();
    }
    bool terminated = false;
    while (!terminated) {
        terminated = true;
        for (it = edges.begin(); it != edges.end();) {
            if (degree[it->first] <= edgeDensity && fixedVertexSet.find(it->first) == fixedVertexSet.end()) {
                terminated = false;
                edges.erase(it++);
                continue;
            }
            it++;
        }
        for (it = edges.begin(); it != edges.end(); it++) {
            std::vector<int> newVector;
            std::vector<int>::iterator it1;
            for (it1 = it->second.begin(); it1 != it->second.end(); it1++) {
                if (edges.find(*it1) != edges.end()) {
                    newVector.push_back(*it1);
                }
            }
            it->second = newVector;
            degree[it->first] = newVector.size();
        }
    }
}

void Graph::shrink() {
    // If the graph is a tree, there is no need to shrink.
    if (m == n - 1) {
        return;
    }
    coreDecomposition();
    // Select a minimal graph with deg(v) >= min(kmax, dmax).
    int mink = ceil(std::max(double(maxCoreNum) / 2, edgeDensity));
    std::unordered_map<int, std::vector<int>>::iterator it;
    for (it = edges.begin(); it != edges.end();) {
        if (coreNum[it->first] < mink) {
            edges.erase(it++);
            continue;
        }
        std::vector<int> newVector;
        std::vector<int>::iterator it1;
        for (it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            if (coreNum[*it1] >= mink) {
                newVector.push_back(*it1);
            }
        }
        edges[it->first] = newVector;
        it++;
    }
    n = edges.size();
}

std::vector<Graph*> Graph::decompose() {
    std::unordered_map<int, int>* parent = new std::unordered_map<int, int>();
    std::unordered_map<int, std::vector<int>>::iterator it;
    for (it = edges.begin(); it != edges.end(); it++) {
        std::vector<int>::iterator it1;
        for (it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            unioN(parent, it->first, *it1);
        }
    }
    std::unordered_map<int, Graph*> CCs;
    for (it = edges.begin(); it != edges.end(); it++) {
        if (parent->find(it->first) == parent->end()) {
            (*parent)[it->first] = it->first;
        }
        int u = find(parent, it->first);
        if (CCs.find(u) == CCs.end()) {
            CCs[u] = new Graph();
            CCs[u]->n = 0;
            CCs[u]->m = 0;
        }
        if (CCs[u]->edges.find(it->first) == CCs[u]->edges.end()) {
            CCs[u]->edges[it->first] = std::vector<int>();
        }
        std::vector<int>::iterator it1;
        for (it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            if (CCs[u]->edges.find(*it1) != CCs[u]->edges.end()) {
                CCs[u]->edges[it->first].push_back(*it1);
                if (CCs[u]->edges.find(*it1) == CCs[u]->edges.end()) {
                    CCs[u]->edges[*it1] = std::vector<int>();
                }
                CCs[u]->edges[*it1].push_back(it->first);
                CCs[u]->m++;
            }
        }
    }
    std::vector<Graph*> CCVector;
    std::unordered_map<int, Graph*>::iterator it1;
    for (it1 = CCs.begin(); it1 != CCs.end(); it1++) {
        it1->second->n = it1->second->edges.size();
        CCVector.push_back(it1->second);
    }
    delete parent;
    return CCVector;
}

Graph::Graph(char* graph_file) {
    std::ifstream fin(graph_file);
    // Reads in vertex num.
    fin >> n;
    // Reads in edges.
    std::string lineStr;
    std::getline(fin, lineStr);
    for (int i = 0; i < n; ++i) {
        std::getline(fin, lineStr);
        std::istringstream lineStream(lineStr);

        int vertex, to;
        lineStream >> vertex;
        edges[vertex] = std::vector<int>();
        while (lineStream >> to) {
            edges[vertex].push_back(to);
        }
    }
}