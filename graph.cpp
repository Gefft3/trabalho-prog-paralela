#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <map>

using namespace std;

typedef struct {
    int *edgelistSize;
    int** edgelist;
    int vertices;
    int *countersPerVertex;

    void initialize(const char* graphDataset) {
        FILE* fp = fopen("teste.txt", "r");
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

void renomearVertices(Graph *graph) {
    map<int, int> vertexMap;
    int newVertexId = 0;

    for (int i = 0; i < graph->getVertices(); i++) {
        vertexMap[i] = newVertexId++;
    }

    for (int i = 0; i < graph->getVertices(); i++) {
        for (int j = 0; j < graph->getEdgelistSize(i); j++) {
            int oldSrc = i;
            int oldDst = graph->getEdge(i, j);
            int newSrc = vertexMap[oldSrc];
            int newDst = vertexMap[oldDst];
            graph->addEdge(newSrc, newDst);
        }
    }
}


int main() {


    Graph* graph = new Graph;
    graph->initialize("teste.txt");

    FILE* fp = fopen("teste.txt", "r");

    int src, dst;
    while(fscanf(fp, "%d %d", &src, &dst) != EOF) {       
        graph->addEdge(src,dst);        
    }

    fclose(fp);
    renomearVertices(graph);


    printf("Vertices: %d\n", graph->getVertices());

    for(int i = 0 ; i < graph->getVertices() ; i++) {
        for(int j = 0 ; j < graph->getEdgelistSize(i) ; j++) {
            printf("%d %d\n", i, graph->getEdge(i, j));
        }
    }

    graph->release();

    return 0;
}