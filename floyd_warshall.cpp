#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <numeric>
#include <iomanip> 
#include <limits>

// Valor padronizado para representar infinito
const int INF = 999999; 

// Função para ler o grafo (já estava correta)
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

// Algoritmo de Floyd-Warshall
std::vector<std::vector<int>> floyd_warshall(const std::vector<std::vector<int>>& grafo, int V) {
    std::vector<std::vector<int>> dist(V, std::vector<int>(V));

    // Passo 1: Inicializa a matriz de distâncias corretamente
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (i == j) {
                dist[i][j] = 0;
            } else if (grafo[i][j] >= INF) {
                dist[i][j] = INF;
            } else {
                dist[i][j] = grafo[i][j];
            }
        }
    }

    // Passo 2: Laços principais do algoritmo
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    return dist;
}

// Função para imprimir a matriz de resultados
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

int main() {
    std::string arquivo_teste = "Entrada 10.txt"; 

    int V;
    std::vector<std::vector<int>> grafo = ler_grafo(arquivo_teste, V);

    if (V == 0) {
        std::cerr << "ERRO: Nao foi possivel ler o grafo do arquivo: " << arquivo_teste << std::endl;
        return 1;
    }
    
    std::vector<double> tempos_execucao;
    for (int i = 0; i < 6; ++i) {
        auto inicio = std::chrono::high_resolution_clock::now();
        floyd_warshall(grafo, V);
        auto fim = std::chrono::high_resolution_clock::now();
        
        if (i > 0) {
            std::chrono::duration<double, std::micro> duracao_us = fim - inicio;
            tempos_execucao.push_back(duracao_us.count());
        }
    }

    double media_tempo = std::accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0.0) / tempos_execucao.size();
    
    std::cout << "--- Algoritmo Floyd-Warshall (C++) ---" << std::endl;
    std::cout << "Arquivo: " << arquivo_teste << " (V=" << V << ")" << std::endl;
    std::cout << "Tempo medio de execucao (us): " << std::fixed << std::setprecision(4) << media_tempo << std::endl;

    if (V <= 25) {
        std::vector<std::vector<int>> matriz_distancias = floyd_warshall(grafo, V);
        imprimir_matriz(matriz_distancias, V);
    }

    return 0;
}