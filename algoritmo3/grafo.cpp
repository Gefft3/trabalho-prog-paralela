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
#include <queue>
#include <atomic>

using namespace std;
using namespace std::chrono;

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

        int contagem_cliques_paralela_balanceada(int k, int n_threads, int roubo_carga);
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

int Graph::contagem_cliques_paralela_balanceada(int k, int n_threads, int roubo_carga) {
        unsigned int num_threads = n_threads;
        if (num_threads == 0) {
            num_threads = 1; 
        }

        vector<vector<int>> cliques_iniciais;
        for (auto v : vertices) {
            cliques_iniciais.push_back({v});
        }

        // Filas de cliques por thread
        vector<queue<vector<int>>> filas_de_cliques(num_threads);

        for (size_t i = 0; i < cliques_iniciais.size(); ++i) {
            filas_de_cliques[i % num_threads].push(cliques_iniciais[i]);
        }

        vector<int> contagens(num_threads, 0);
        vector<thread> threads;

        set<vector<int>> cliques_verificados;
        mutex clique_mutex;
        mutex fila_mutex;
        atomic<bool> trabalho_disponivel(true); 

        for (unsigned int tid = 0; tid < num_threads; ++tid) {
            threads.emplace_back([&, tid]() {
                while (trabalho_disponivel) {
                    vector<int> clique;
                    bool clique_obtido = false;

                    // tenta achar carga de trabalho na propria fila
                    {
                        lock_guard<mutex> lock(fila_mutex);
                        if (!filas_de_cliques[tid].empty()) {
                            clique = filas_de_cliques[tid].front();
                            filas_de_cliques[tid].pop();
                            clique_obtido = true;
                        }
                    }

                    // procura na propria fila primeiro, e depois nas filas de outras threads (caso não tenha nenhuma carga de trabalho na propria)
                    if (!clique_obtido) {
                        for (unsigned int i = 0; i < num_threads; ++i) {
                            if (i != tid) {
                                lock_guard<mutex> lock(fila_mutex);
                                if (!filas_de_cliques[i].empty()) {
                                    clique = filas_de_cliques[i].front();
                                    filas_de_cliques[i].pop();
                                    clique_obtido = true;
                                    break;
                                }
                            }
                        }
                    }

                    // encerrar thread se não houver mais trabalho em nhenhuma thread
                    if (!clique_obtido) {
                        trabalho_disponivel = false;
                        return;
                    }

                    {
                        lock_guard<mutex> lock(clique_mutex);
                        if (clique_ja_existe(cliques_verificados, clique)) {
                            continue;
                        }
                        cliques_verificados.insert(clique);
                    }

                    int tamanho_clique = clique.size();
                    if (tamanho_clique == k) {
                        contagens[tid]++;
                        continue;
                    }

                    int ultimo_vertice = clique.back();

                    for (int vertice : clique) {
                        vector<int> vizinhos_atual = getNeighbours(vertice);

                        for (int vizinho : vizinhos_atual) {
                            if (vizinho > ultimo_vertice && formar_clique(vizinho, clique)) {
                                vector<int> nova_clique = clique;
                                nova_clique.push_back(vizinho);
                                //zona crítica
                                {
                                    lock_guard<mutex> lock(fila_mutex);
                                    filas_de_cliques[tid].push(nova_clique);
                                }
                            }
                        }
                    }
                }
            });
        }

        for (auto &th : threads) {
            th.join();
        }

        int total_count = 0;
        for (int c : contagens) {
            total_count += c;
        }

        return total_count;
    }


int main() {
    string dataset = "../datasets/ca_astroph.edgelist";
    // string dataset = "teste";
    vector<pair<int, int>> edges = rename(dataset);
    Graph* g = new Graph(edges);
    // g->printar_grafo();
    
    auto start = high_resolution_clock::now();
    int result = g->contagem_cliques_paralela_balanceada(3, 4, 20);
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    cout << "Resultado: " << result << endl;
    cout << "Tempo de execução: " << duration.count() << " segundos" << endl;
    g->release();
    delete g;
}