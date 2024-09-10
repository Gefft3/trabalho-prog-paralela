# Trabalho Prático 1 – Programação Paralela (2024/02)

## Objetivo

O objetivo deste trabalho prático é permitir o projeto e implementação de um algoritmo
paralelo em uma arquitetura de memória compartilhada convencional (CPU multicore) utilizando o
modelo de programação Pthread em C/C++.

## Descrição do problema

Neste trabalho vocês projetarão e implementarão algoritmos que solucionem o problema
da contagem de cliques em grafos não direcionados. Por definição, um grafo G pode ser
representado por um conjunto de vértices denotado V(G), e por um conjunto de arestas E(G) que
conecta vértices em V(G). No nosso contexto, cada vértice de um grafo é identificado unicamente
utilizando um inteiro no intervalo [0 … V(G) – 1], e as arestas do grafo não possuem direção. A
Figura 1 apresenta um grafo G conexo e não direcionado contendo 8 vértices e 11 arestas.

![image](https://github.com/user-attachments/assets/5928187f-7d5f-456c-ac0c-ccfb2058e44e)

Figura 1. Exemplo de grafo conexo não direcionado com 2 cliques.

O grafo da Figura 1 é dito conexo pois, dado um vértice v qualquer do grafo, existe um
caminho que conecta v a qualquer outro vértice do grafo. Os subgrafos da Figura que estão
realçados em vermelho e verde são chamados de cliques, pois todos os vértices pertencentes a essas
subgrafos estão conectados entre si. Uma k-clique é uma clique contendo k vértices. Ou seja, a
clique em vermelho é uma 3-clique, enquanto a clique em verde é uma 4-clique.
Dado um grafo de entrada G conexo e não direcionado, você construirá algoritmos para
contar a quantidade total de k-cliques em G. O pseudocódigo abaixo (estilo Python) apresenta um
algoritmo genérico para a contagem de k-cliques em um grafo G conexo e não direcionado,
utilizando a abordagem BFS (Breadth-First Search).

    def contagem_de_cliques_serial(g:Grafo, k:int):
        cliques = []
            for each v in g.V(G):
                cliques.append([v])
        
            contador = 0
            while not empty cliques:
                clique = cliques.pop()
                
                // A clique atual já tem k vértices
                if(len(clique) == k):
                    contador+=1
                    continue
                
                ultimo_vertice = clique[len(clique)-1]
                for each vertice in clique:
                    for each vizinho in (adjacência de vertice):
                        if (vizinho not in clique) and
                        (vizinho se conecta a todos os vértices de clique) and
                        (vizinho > ultimo_vertice):
                            nova_clique = clique.clone()
                            nova_clique.append(vizinho)
                            cliques.push(nova_clique)
        return contador
