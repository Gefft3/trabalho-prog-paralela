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
#include <thread>
#include <mutex>

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

        int contagem_cliques_paralela(int k, int n_threads);
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

        void printar_clique(vector<int> clique) {
            for (auto v : clique) {
                cout << v << " ";
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

    //cout << "vertice vizinho: " << vertex << endl;
    //cout << "se conecta a todos (tem que): " << cond1 << endl;
    //cout << "já está na clique (não pode): " << cond2 << endl;

    bool condf = (cond1 && !cond2); 
    //cout << "decisão: " << condf << endl;
    return condf;
}

bool clique_ja_existe(const std::set<std::vector<int>>& cliques, const std::vector<int>& clique) {
    return cliques.find(clique) != cliques.end();
}

int Graph::contagem_cliques_paralela(int k, int n_threads) {
    unsigned int num_threads = n_threads;

    if (num_threads == 0) {
        num_threads = 1; 
    }

    // Criação dos cliques iniciais com um vértice
    vector<vector<int>> cliques_iniciais;
    for (auto v : vertices) {
        cliques_iniciais.push_back({v});
    }

    // Dividindo os cliques iniciais igualmente entre as threads
    vector<vector<vector<int>>> cliques_por_thread(num_threads);
    size_t num_cliques = cliques_iniciais.size();
    size_t cliques_por_thread_size = num_cliques / num_threads;
    size_t excesso = num_cliques % num_threads;

    // Distribuição estática dos cliques
    size_t indice = 0;
    for (unsigned int tid = 0; tid < num_threads; ++tid) {
        size_t num_para_thread = cliques_por_thread_size + (tid < excesso ? 1 : 0);
        for (size_t j = 0; j < num_para_thread; ++j) {
            cliques_por_thread[tid].push_back(cliques_iniciais[indice++]);
        }
    }

    vector<int> contagens(num_threads, 0);
    vector<thread> threads;

    for (unsigned int tid = 0; tid < num_threads; ++tid) {
        threads.emplace_back([&, tid]() {
            set<vector<int>> cliques;
            cliques.insert(cliques_por_thread[tid].begin(), cliques_por_thread[tid].end());
            int &count = contagens[tid];

            while (!cliques.empty()) {
                
                vector<int> clique = *cliques.begin();
                cliques.erase(cliques.begin());


                int tamanho_clique = clique.size();
                if (tamanho_clique == k) {
                    count++;
                    continue;
                }

                int ultimo_vertice = clique.back();

                for (int vertice : clique) {
                    vector<int> vizinhos_atual = getNeighbours(vertice);

                    for (int vizinho : vizinhos_atual) {
                        if (vizinho > ultimo_vertice && formar_clique(vizinho, clique)) {
                            vector<int> nova_clique = clique;
                            nova_clique.push_back(vizinho);
                            cliques.insert(nova_clique);
                        }
                    }
                }
            }
        });
    }

    // Espera por todas as threads terminarem
    for (auto &th : threads) {
        th.join();
    }

    // Soma os resultados de todas as threads
    int total_count = 0;
    for (int c : contagens) {
        total_count += c;
    }

    return total_count;
}

int main() {
    string dataset = "../datasets/citeseer.edgelist";
    // string dataset = "teste";
    vector<pair<int, int>> edges = rename(dataset);
    Graph* g = new Graph(edges);
    // g->printar_grafo();
    
    cout << g->contagem_cliques_paralela(3,8) << endl;
    g->release();
    delete g;
}