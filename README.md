Sim, claro! Transformar o relatório em um README.md para o GitHub é uma ótima ideia. O formato Markdown é mais direto e ideal para apresentar um projeto de software.

Abaixo está uma versão do seu relatório adaptada para o formato README.md. Ele mantém toda a análise e os resultados, mas com uma estrutura mais adequada para o GitHub, incluindo as instruções de como compilar e executar o código.

Você pode copiar este texto todo e colar em um novo arquivo chamado README.md no seu repositório.

2º Trabalho de PAA: Floyd-Warshall vs. Dijkstra para Caminhos Mínimos

Análise teórica e empírica de algoritmos para o problema de encontrar o caminho mínimo entre todos os pares de vértices em um grafo.

Sobre o Projeto

Este trabalho, desenvolvido para a disciplina de Projeto e Análise de Algoritmos (PAA) na UNIOESTE - Campus Cascavel, compara duas soluções clássicas para o problema de encontrar o caminho mínimo entre todos os pares de vértices (All-Pairs Shortest Path - APSP):

    Algoritmo de Floyd-Warshall: Uma abordagem baseada em programação dinâmica.

    Algoritmo de Dijkstra (V vezes): Uma abordagem que executa o algoritmo guloso de Dijkstra a partir de cada vértice do grafo.

O objetivo é analisar o desempenho cronológico das duas estratégias e validar a correção dos resultados, comparando a teoria de complexidade com a performance observada na prática. A implementação foi realizada em C++.

Algoritmos Implementados e Análise de Complexidade

1. Floyd-Warshall

O algoritmo de Floyd-Warshall utiliza três laços de repetição aninhados para iterar sobre todos os vértices i, j, e k, testando se o caminho de i para j pode ser melhorado passando pelo vértice intermediário k. Sua simplicidade estrutural resulta em uma complexidade de tempo consistente e independente da densidade do grafo.

    Complexidade Assintótica: $O(V^3)$

2. Dijkstra (Executado V vezes)

Esta abordagem resolve o problema executando o algoritmo de Dijkstra (que encontra o caminho mínimo de uma única fonte) para cada um dos V vértices do grafo. A implementação utiliza um vetor simples para a fila de prioridades.

    Complexidade de 1 Execução (Dijkstra com Vetor): $O(V^2)$

    Complexidade Total (V Execuções): $V \times O(V^2) = O(V^3)$

Teoricamente, para grafos densos (representados por matriz de adjacência), ambas as abordagens possuem a mesma complexidade assintótica.

Como Compilar e Executar

Pré-requisitos

    Um compilador C++ (como o G++)

Instruções

    Clone o repositório.

    Navegue até a pasta do projeto pelo terminal.

    Compile os arquivos .cpp individualmente:
    # Para compilar o Floyd-Warshall
g++ -std=c++11 -o floyd floyd_warshall.cpp

# Para compilar o Dijkstra
g++ -std=c++11 -o dijkstra dijkstra_v_vezes.cpp

Execute os programas. Lembre-se de alterar o nome do arquivo de entrada dentro do código (main.cpp) antes de cada execução.

# Para executar o Floyd-Warshall
./floyd.exe

# Para executar o Dijkstra
./dijkstra.exe

Resultados

Os experimentos foram executados em 15 conjuntos de dados (grafos), com o número de vértices V variando de 10 a 1.500. A tabela abaixo resume os tempos médios de execução obtidos.

Tamanho (V)	    Tempo (Floyd-Warshall)	            Tempo (Dijkstra V vezes)
    10	              10,60 µs	                              5,00 µs
    25	              100,60 µs	                              42,60 µs
    50	              753,60 µs	                              287,80 µs
    75	              2,45 ms	                                0,88 ms
    100	              5,92 ms	                                2,03 ms
    150	              19,86 ms	                              6,47 ms
    200	              46,13 ms	                              15,65 ms
    250	              90,63 ms	                              29,57 ms
    300	              156,97 ms	                              51,52 ms
    400	              377,30 ms	                              122,85 ms
    500	              0,74 s	                                 0,24 s
    650	              1,77 s	                                 0,57 s
    800	              3,30 s	                                 1,07 s
    1.000	            6,46 s	                                 2,10 s
    1.500	            21,76 s	                                 7,07 s

Em todos os testes, foi validado que ambas as implementações produziram matrizes de distância idênticas, confirmando sua correção.

Análise dos Resultados

Embora ambas as estratégias possuam a mesma complexidade teórica de `O(V3), a análise empírica revela que a abordagem de executar o Dijkstra V vezes foi consistentemente mais rápida, chegando a ser 3 vezes mais veloz para os maiores grafos.

Essa diferença de performance se deve às operações internas de cada algoritmo. O Floyd-Warshall executa um número fixo e massivo de somas e comparações, enquanto o Dijkstra, mesmo repetido V vezes, pode se beneficiar da estrutura do grafo, e suas operações internas (busca pelo vértice de distância mínima e relaxamento de arestas) são, na prática, computacionalmente menos custosas.

Autores

    João Gabriel Fazio Pauli

    Lucas Batista Deinzer Duarte

    Fernando Seiji Onoda Inomata
