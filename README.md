# Análise de Algoritmos: Floyd-Warshall vs. Dijkstra (V vezes)

Este repositório contém um projeto em C++ desenvolvido para a disciplina de **Projeto de Análise de Algoritmos**.

O objetivo principal é comparar empiricamente o desempenho de dois métodos clássicos para resolver o problema de **Todos-os-Pares de Caminhos Mínimos** (All-Pairs Shortest Path - APSP).

## Sobre o Projeto

O programa implementa e cronometra os seguintes algoritmos:

1.  **Floyd-Warshall:** Um algoritmo de programação dinâmica que resolve o problema APSP com complexidade de tempo $O(V^3)$.
2.  **Dijkstra (Executado V vezes):** Uma solução alternativa que consiste em executar o algoritmo de Dijkstra (de fonte única) $V$ vezes, uma para cada vértice do grafo. A complexidade teórica total é $O(V \times (V^2))$ ou $O(V^3)$ para a implementação simples baseada em array utilizada.

## Metodologia

O script `main.cpp` realiza as seguintes etapas:

1.  **Leitura:** Lê uma série de grafos (representados por matrizes de adjacência) de arquivos de entrada (ex: `Entrada 10.txt`, `Entrada 50.txt`, etc.).
2.  **Cronometragem:** Para cada grafo, executa e mede o tempo médio (em microssegundos) de ambos os algoritmos. É utilizada uma média de 5 execuções (descartando a primeira para aquecimento).
3.  **Validação:** Calcula a soma total de todas as distâncias mínimas para ambos os algoritmos, garantindo que os resultados sejam idênticos.
4.  **Exportação:** Salva os resultados de tempo e validação em dois arquivos na pasta `Resultados/`:
    * `resultados_finais.csv`: Para fácil visualização e geração de gráficos.
    * `tabela_latex.txt`: Bloco de código pronto para ser copiado em um relatório LaTeX.
5.  **Saída de Matrizes:** Para grafos pequenos ($V < 100$), as matrizes de distância resultantes são salvas em `Resultados/Matrizes/` para verificação manual.

## Como Compilar e Executar

Este projeto utiliza a biblioteca `<filesystem>` e requer um compilador com suporte a **C++17** ou superior.

### Pré-requisitos

* Um compilador C++17 (ex: `g++`, `clang++`)
* Os arquivos de entrada (`Entrada X.txt`) no mesmo diretório do executável.

### Compilação

```bash
# Exemplo usando g++
g++ -std=c++17 -o analisador main.cpp
