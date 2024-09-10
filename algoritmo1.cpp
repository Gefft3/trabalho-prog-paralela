#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>



using namespace std;

typedef struct {
    int *edgelistSize;
    int** edgelist;
    int vertices;
    int *countersPerVertex;

    void initialize(const char* graphDataset) {
        FILE* fp = fopen("./datasets/citeseer.edgelist", "r");
        int src, dst;
        int maxVertexId = -1;
        while(fscanf(fp, "%d %d", &src, &dst) != EOF) {
            if(src > maxVertexId)
                maxVertexId = src;
            if(dst > maxVertexId)
                maxVertexId = dst;
        }
        vertices = maxVertexId+1;    

        rewind(fp);
        edgelistSize = (int*)malloc(vertices*sizeof(int));
        while(fscanf(fp, "%d %d", &src, &dst) != EOF) {
            edgelistSize[src]++;
        }

        // Memory allocation
        edgelist = (int**)malloc(vertices * sizeof(int*));
        for(int i = 0 ; i < vertices ; i++) {
            edgelist[i] = (int*)malloc(edgelistSize[i] * sizeof(int));
        }

        countersPerVertex = (int*)malloc(vertices*sizeof(int));
        memset(countersPerVertex, 0, vertices*sizeof(int));

        rewind(fp);
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
        edgelist[src][countersPerVertex[src]++] = dst;
    }

    //TODO
    bool isNeighbour(int src, int dst) {
        return true;
    }
    void release() {
        for(int i = 0 ; i < vertices ; i++)
            free(edgelist[i]);
        free(edgelist);
        // TODO Release the other structures.
    }
} Graph;


    

int contagem_cliques_serial(Graph graph, int k){
    
    vector<std::vector<int>> cliques;

    for (int v = 0; v < graph.getVertices(); v++) {
        vector<int> clique;
        clique.push_back(v);
        cliques.push_back(clique);
    }

    int count = 0;

    while (cliques.size() > 0) {
        vector<int> clique = cliques.back();
        cliques.pop_back();

        if (clique.size() == k) {
            count++;
            continue;
        }

        for (int v = clique.back() + 1; v < graph.getVertices(); v++) {
            bool isClique = true;
            for (int i = 0; i < clique.size(); i++) {
                if (!graph.isNeighbour(v, clique[i])) {
                    isClique = false;
                    break;
                }
            }

            if (isClique) {
                vector<int> newClique = clique;
                newClique.push_back(v);
                cliques.push_back(newClique);
            }
        }
    }

    return count;
}



int main() {
    Graph* graph = new Graph;
    graph->initialize("./datasets/citeseer.edgelist");

    FILE* fp = fopen("./datasets/citeseer.edgelist", "r");

    int src, dst;
    while(fscanf(fp, "%d %d", &src, &dst) != EOF) {       
        graph->addEdge(src,dst);        
    }

    fclose(fp);

    int cliques = 0;

    cliques = contagem_cliques_serial(*graph, 3);

    printf("Cliques: %d\n", cliques);  

    graph->release();

    return 0;
}