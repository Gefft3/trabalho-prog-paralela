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

Algoritmo 2. Contagem de cliques paralela com divisão de trabalho fixa.

O principal problema do Algoritmo 2 é que a divisão de trabalho fixa entre as threads pode
gerar o desbalancamento de carga entre as threads, dado que não é possível prever onde estarão as
cliques de um grafo. Para avaliar o impacto desse desbalanceamento, você desenvolverá uma
terceira versão que estenderá o Algoritmo 2 para permitir um balanceamento de carga
parametrizado. Essa versão está representada no Algoritmo 3.


    def contagem_de_cliques_paralela1(g:Grafo, k:int, t:int, r:int):
        cliques = []
        for each v in g.V(G):
            cliques.append([v])

        trabalhos_threads = divida vetor cliques em t conjuntos do mesmo tamanho

        Crie t threads com o seguinte código:
            contador = 0
            cliques = trabalhos_threads[tid]
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
                if empty cliques:
                    cliques = r cliques “roubadas” de outra thread
        return contador

Algoritmo 3. Contagem de cliques paralela com divisão de trabalho fixa e
balanceamento de carga

Note que, no Algoritmo 3, quando uma thread fica sem trabalho a realizar (linha 28), ela
acessa o conjunto de trabalhos de uma outra thread e “rouba” um conjunto de r trabalhos. Os
critérios utilizados para escolher qual(is) thread(s) terá(ão) seus trabalhos “roubados” ficam à sua escolha. Não se esqueça de utilizar um mecanismo de sincronização durante o acesso às tarefas de outra thread.

## Requisitos de Entrega

Neste trabalho você deve implementar e entregar os seguintes artefatos:

<ol>
    <li>Implementação do algoritmo 1</li>
    <li>Implementação do algoritmo 2</li>
    <li>Implementação do algoritmo 3</li>
    <li>Conferência de todos os resultados produzidos pelas implementações dos Algoritmos 1, 2 e
3 em relação à corretude da quantidade de cliques geradas considerando os seguintes
datasets:</li>

![image](https://github.com/user-attachments/assets/ae023850-97c3-4fb4-ba12-30a946f1a909)

<li>Descrição textual da estratégia adotada no Algoritmo 3 para escolher qual a thread que terá
suas tarefas “roubadas”;</li>
    <li>Análise dos tempos de execução dos Algoritmos 1, 2 e 3. Nessa análise você deve medir
apenas o tempo de execução gasto para a geração das contagens de cliques, e não o tempo
total de execução do arquivo executável correspondente ao Algoritmo. Você deve variar o
tamanho das cliques buscadas até que o programa chegue à um tempo de execução máximo
de 4 horas. </li>
    <li>Em relação à sua implementação do Algoritmo 3, você deve também prover uma análise
experimental variando o valor de r. Nesse experimento você deve tentar encontrar o ponto
em que o valor de r deixa de prover ganhos de desempenho;</li>
    <li>Arquivo .zip contendo todos os códigos implementados (itens 1, 2, 3 e 4);</li>
    <li>Relatório em PDF contemplando os itens 5, 6, 7, 8 e 9.</li>
</ol>

## Entrega

Todos os artefatos descritos no item anterior devem ser entregues até o dia 01 de Setembro
de 2024, via AVA.
