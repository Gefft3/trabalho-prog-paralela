#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

int main() {
    ifstream inputFile("teste.txt");
    ofstream outputFile("teste.txt");
    map<int, int> nodeMap;
    vector<pair<int, int>> edges;
    int nodeCounter = 1;
    
    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
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

    for (const auto& edge : edges) {
        outputFile << edge.first << " " << edge.second << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "O grafo foi renomeado com sucesso!" << endl;
    return 0;
}
