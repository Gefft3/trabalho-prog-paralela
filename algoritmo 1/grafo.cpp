#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <thread>
#include <map>
#include <vector>
#include "sort.h"
#include <algorithm>
#include <set>

using namespace std;

class Graph {
    public:

        vector<pair<int, int>> arestas;
        vector<set<int>> vizinhos; 
        set<int> vertices; 

        Graph(vector<pair<int, int>> edgeList) {
            for (auto edge : edgeList) {
                const int tamanho_vizinhos = vizinhos.size();
                if (tamanho_vizinhos <= max(edge.first, edge.second)) {
                    vizinhos.resize(max(edge.first, edge.second) + 1);
                }
                arestas.push_back(edge);
                vertices.insert(edge.first); 
                vertices.insert(edge.second); 
                vizinhos[edge.first].insert(edge.second); 
                vizinhos[edge.second].insert(edge.first); 
            }
        }

        int contagem_cliques_serial(int k);
        bool esta_na_clique(int vertex, vector<int> clique);
        bool se_conecta_a_todos_os_vertices_da_clique(int vertex, vector<int> clique);
        bool formar_clique(int vertex, vector<int> clique);

        vector<int> getNeighbours(int vertex) {
            vector<int> neighbours;
            for (int neighbour : vizinhos[vertex]) {
                neighbours.push_back(neighbour);
            }
            return neighbours;
        }

        bool isNeighbour(int vertex, int neighbour) {
            return vizinhos[vertex].find(neighbour) != vizinhos[vertex].end();
        }

        void printar_grafo() {
            for (auto v : vertices) {
                cout << v << ": ";
                for (auto n : vizinhos[v]) {
                    cout << n << " ";
                }
                cout << endl;
            }
        }

        void release() {
            arestas.clear();
            vertices.clear();
            vizinhos.clear();
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

    return edges;
}

bool Graph::esta_na_clique(int vertex, vector<int> clique) {
    for (size_t i = 0; i < clique.size(); i++) {
        if (vertex == clique[i]) {
            return true;
        }
    }
    return false;
}

bool Graph::se_conecta_a_todos_os_vertices_da_clique(int vertex, vector<int> clique) {
    for (int v : clique) {
        if (!isNeighbour(vertex, v)) {
            return false;
        }
    }
    return true;
}

bool Graph::formar_clique(int vertex, vector<int> clique) {
    bool cond1 = se_conecta_a_todos_os_vertices_da_clique(vertex, clique);
    bool cond2 = esta_na_clique(vertex, clique);

    cout << "Vizinho: " << vertex << endl;
    cout << "cond1 (se conecta a todos): " << cond1 << endl;
    cout << "cond2 (já está na clique): " << cond2 << endl;

    return (cond1 && !cond2);
}



int Graph::contagem_cliques_serial(int k) {
    vector<vector<int>> cliques;

    for(auto v: vertices) {
        cliques.push_back({v});
    }

    int count = 0;

    while(!cliques.empty()){
        vector<int> clique;
        
        clique = cliques.back();
        cliques.pop_back();
    
        int tamanho_clique = clique.size();
        if(tamanho_clique == k){
            count++;
            continue;
        }
        
        int last_vertex = clique[clique.size() -  1];
        
        for(int vertice : clique){
            vector<int> vizinhos_atual = getNeighbours(vertice); 
            
            for(int vizinho: vizinhos_atual){
                if(formar_clique(vizinho, clique) && vizinho > last_vertex){
                    vector<int> nova_clique = clique;
                    nova_clique.push_back(vizinho);
                    cliques.push_back(nova_clique);
                }
            }
        } 
    }

    return count;
    
}

int main() {
    string dataset = "../datasets/citeseer.edgelist";
    vector<pair<int, int>> edges = rename(dataset);
    Graph* g = new Graph(edges);
    // g->printar_grafo();
    cout << g->contagem_cliques_serial(3) << endl;
    g->release();
    delete g;
}