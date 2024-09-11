#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <map>
#include <vector>

using namespace std;

class graph {
    int* edgelistSize;
    int** edgelist;
    int vertices;
    int* countersPerVertex;

public:
    graph(vector<pair<int, int>> edges) {
        int src, dst;
        int maxVertexId = -1;

        for (const auto& edge : edges) {
            src = edge.first;
            dst = edge.second;
            if (src > maxVertexId)
                maxVertexId = src;
            if (dst > maxVertexId)
                maxVertexId = dst;
        }

        vertices = maxVertexId + 1;

        edgelistSize = (int*)malloc(vertices * sizeof(int));
        memset(edgelistSize, 0, vertices * sizeof(int));  

        for (const auto& edge : edges) {
            src = edge.first;
            edgelistSize[src]++;
        }

        edgelist = (int**)malloc(vertices * sizeof(int*));
        for (int i = 0; i < vertices; i++) {
            edgelist[i] = (int*)malloc(edgelistSize[i] * sizeof(int));
        }

        countersPerVertex = (int*)malloc(vertices * sizeof(int));
        memset(countersPerVertex, 0, vertices * sizeof(int));

        for (const auto& edge : edges) {
            src = edge.first;
            dst = edge.second;
            addEdge(src, dst);  
        }
    }

    void infos() {
        cout << "Vertices: " << vertices << endl;

        for (int i = 0; i < vertices; i++) {
            cout << "EdgelistSize[" << i << "]: " << edgelistSize[i] << endl;
        }
    }

    int getVertices() {
        return vertices;
    }

    int getEdgelistSize(int src) {
        return edgelistSize[src];
    }

    int getEdge(int src, int pos) {
        return edgelist[src][pos];
    }

    void addEdge(int src, int dst) {
        if (countersPerVertex[src] < edgelistSize[src]) {  
            edgelist[src][countersPerVertex[src]++] = dst;
        }

        if (countersPerVertex[dst] < edgelistSize[dst]) { 
            edgelist[dst][countersPerVertex[dst]++] = src;
        }
    }

    bool isNeighbour(int src, int dst) {
        for (int i = 0; i < edgelistSize[src]; i++) {
            if (edgelist[src][i] == dst)
                return true;
        }
        for (int i = 0; i < edgelistSize[dst]; i++) {
            if (edgelist[dst][i] == src)
                return true;
        }
        return false;
    }

    void printar_grafo() {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < edgelistSize[i]; j++) {
                cout << i << " " << edgelist[i][j] << endl;
            }
        }
    }

    void release() {
        for (int i = 0; i < vertices; i++)
            free(edgelist[i]);
        free(edgelist);
        free(edgelistSize);
        free(countersPerVertex);
    }
};


vector<pair<int, int>> rename(const string& dataset) {
    
    ifstream inputFile(dataset);
    map<int, int> nodeMap;
    vector<pair<int, int>> edges;
    int nodeCounter = 0;
    
    if (!inputFile.is_open()) {
        cerr << "nao abriu, arquivo: " << dataset << endl;
    }

    int u, v;

    while (inputFile >> u >> v) {
        if (nodeMap.find(u) == nodeMap.end()) {
            nodeMap[u] = nodeCounter++;
        }
        if (nodeMap.find(v) == nodeMap.end()) {
            nodeMap[v] = nodeCounter++;
        }
        edges.push_back({nodeMap[u], nodeMap[v]});
    }


    inputFile.close();
    
    // cout << "renomeou" << endl;

    return edges;
}



int main(){


    string dataset = "teste"; 

    vector<pair<int, int>> edges = rename(dataset);

    graph* g = new graph(edges);
    

    g->infos();
    g->printar_grafo();

    g->release();
    delete g;

    return EXIT_SUCCESS;
}