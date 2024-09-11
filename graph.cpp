// Grafo de exemplo do professor

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

typedef struct {

    int* edgelistSize; // Um ponteiro para um array que armazena o número de arestas (vizinhos) para cada vértice.
    int** edgelist; //Um ponteiro para um array de ponteiros, onde cada ponteiro armazena um array de vértices adjacentes (arestas) para um dado vértice.   
    int vertices;
    int* countersPerVertex; // Um ponteiro para um array que armazena o número de arestas (vizinhos) para cada vértice.

    // Initialize the graph
    void initialize(const char* graphDataset) {
        int src, dst;
        int maxVertexId = -1;
        
        FILE* fp = fopen("teste", "r");
        
        if(fp == NULL || !fp) {
            printf("Error: Unable to open the file\n");
            exit(1);
        }

        // Get the number of vertices
        while(fscanf(fp, "%d %d", &src, &dst) != EOF) {
            if(src > maxVertexId)
                maxVertexId = src;
            if(dst > maxVertexId)
                maxVertexId = dst;
        }
        vertices = maxVertexId+1;    

        // Rewind the file pointer
        rewind(fp);

        // Initialize the edgelistSize array
        edgelistSize = (int*)malloc(vertices*sizeof(int));
        while(fscanf(fp, "%d %d", &src, &dst) != EOF) {
            edgelistSize[src]++;
        }

        // Memory allocation
        edgelist = (int**)malloc(vertices * sizeof(int*));
        for(int i = 0 ; i < vertices ; i++) {
            edgelist[i] = (int*)malloc(edgelistSize[i] * sizeof(int));
        }
        // Initialize the countersPerVertex
        countersPerVertex = (int*)malloc(vertices*sizeof(int));
        memset(countersPerVertex, 0, vertices*sizeof(int));

        rewind(fp);
    }

    void infos(){
        printf("Vertices: %d\n", vertices);
        
        printf("EdgelistSize: %n\n", edgelistSize);
        for(int i = 0 ; i < vertices ; i++) {
            for(int j = 0 ; j < edgelistSize[i] ; j++) {
                printf("EdgelistSize[%d]: %d\n", i, edgelistSize[i]);
            }
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
        edgelist[src][countersPerVertex[src]++] = dst;
    }

    // Check if the two vertices are neighbours
    bool isNeighbour(int src, int dst) {
        for(int i = 0 ; i < edgelistSize[src] ; i++) {
            if(edgelist[src][i] == dst)
                return true;
        }
        for(int i = 0 ; i < edgelistSize[dst] ; i++) {
            if(edgelist[dst][i] == src)
                return true;
        }
        return false;    
    }

    // Release the memory
    void release() {
        for(int i = 0 ; i < vertices ; i++)
            free(edgelist[i]);
        
        free(edgelist);
        free(edgelistSize);
        free(countersPerVertex);
    }
} Graph;

int main() {
    Graph* graph = new Graph;
    graph->initialize("teste");

    FILE* fp = fopen("teste", "r");

    int src, dst;
    while(fscanf(fp, "%d %d", &src, &dst) != EOF) {       
        graph->addEdge(src,dst);        
    }

    // Close the file
    fclose(fp);

    // Print the graph
    for(int i = 0 ; i < graph->getVertices() ; i++) {
        for(int j = 0 ; j < graph->getEdgelistSize(i) ; j++) {
            printf("%d %d\n", i, graph->getEdge(i, j));
        }
    }

    graph->infos();


    // Free the memory
    graph->release();

    return 0;
}