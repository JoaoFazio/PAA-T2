#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <limits>

const int INF = 999999;

// Função para ler o grafo (idêntica à anterior)
std::vector<std::vector<int>> ler_grafo(const std::string& path, int& V) {
    std::ifstream arquivo(path);
    if (!arquivo.is_open()) {
        V = 0;
        return {};
    }

    arquivo >> V;
    if (V == 0) return {};

    std::vector<std::vector<int>> grafo(V, std::vector<int>(V));
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (!(arquivo >> grafo[i][j])) {
                std::cerr << "ERRO: Falha ao ler a matriz na posicao (" << i << "," << j << ")" << std::endl;
                V = 0;
                return {};
            }
        }
    }
    return grafo;
}

// Função para imprimir a matriz de resultados (idêntica à anterior)
void imprimir_matriz(const std::vector<std::vector<int>>& matriz, int V) {
    std::cout << "\n--- Matriz de Distancias Finais ---" << std::endl;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (matriz[i][j] >= INF) {
                std::cout << std::setw(7) << "INF";
            } else {
                std::cout << std::setw(7) << matriz[i][j];
            }
        }
        std::cout << std::endl;
    }
}

// Função auxiliar para encontrar o vértice com a menor distância (não visitado)
int encontrar_min_dist(const std::vector<int>& dist, const std::vector<bool>& visitado, int V) {
    int min_dist = INF;
    int min_index = -1;

    for (int v = 0; v < V; ++v) {
        if (!visitado[v] && dist[v] < min_dist) {
            min_dist = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Implementação do Dijkstra com Vetor Simples O(V^2)
std::vector<int> dijkstra_simples(const std::vector<std::vector<int>>& grafo, int V, int origem) {
    std::vector<int> dist(V, INF);
    std::vector<bool> visitado(V, false);
    dist[origem] = 0;

    for (int count = 0; count < V; ++count) {
        int u = encontrar_min_dist(dist, visitado, V);
        if (u == -1) break;

        visitado[u] = true;

        for (int v = 0; v < V; ++v) {
            if (!visitado[v] && grafo[u][v] != 0 && dist[u] != INF && dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
            }
        }
    }
    return dist;
}

// Função que executa o Dijkstra V vezes
std::vector<std::vector<int>> dijkstra_v_vezes(const std::vector<std::vector<int>>& grafo, int V) {
    std::vector<std::vector<int>> todos_os_caminhos(V);
    for (int i = 0; i < V; ++i) {
        todos_os_caminhos[i] = dijkstra_simples(grafo, V, i);
    }
    return todos_os_caminhos;
}

int main() {
    std::string arquivo_teste = "Entrada 10.txt"; // <-- MUDE AQUI O NOME DO ARQUIVO

    int V;
    std::vector<std::vector<int>> grafo = ler_grafo(arquivo_teste, V);

    if (V == 0) {
        std::cerr << "ERRO: Nao foi possivel ler o grafo do arquivo: " << arquivo_teste << std::endl;
        return 1;
    }

    std::vector<double> tempos_execucao;
    for (int i = 0; i < 6; ++i) {
        auto inicio = std::chrono::high_resolution_clock::now();
        dijkstra_v_vezes(grafo, V);
        auto fim = std::chrono::high_resolution_clock::now();
        
        if (i > 0) {
            std::chrono::duration<double, std::micro> duracao_us = fim - inicio;
            tempos_execucao.push_back(duracao_us.count());
        }
    }

    double media_tempo = std::accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0.0) / tempos_execucao.size();
    
    std::cout << "--- Algoritmo Dijkstra V vezes (C++) ---" << std::endl;
    std::cout << "Arquivo: " << arquivo_teste << " (V=" << V << ")" << std::endl;
    std::cout << "Tempo medio de execucao (us): " << std::fixed << std::setprecision(4) << media_tempo << std::endl;
    
    if (V <= 25) {
        std::vector<std::vector<int>> matriz_distancias = dijkstra_v_vezes(grafo, V);
        imprimir_matriz(matriz_distancias, V);
    }

    return 0;
}