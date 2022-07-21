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

void Graph::shrink() {
    // If the graph is a tree, there is no need to shrink.
    if (m == n - 1) {
        return;
    }
    // Compute core numbers `k` and approximated maximum density `dmax`.
    /*
        TBD
    */
    // Select a minimal graph with deg(v) >= min(kmax, dmax).
    /*
        TBD
    */
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