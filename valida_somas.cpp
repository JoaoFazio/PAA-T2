#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <numeric>
#include <iomanip>
#include <limits>

// Valor padronizado para representar infinito
const int INF = 999999;

// Estrutura para o grafo
struct Grafo
{
    int V = 0;
    std::vector<std::vector<int>> matriz;
};

// Função para ler o grafo (robusta)
Grafo ler_grafo(const std::string &path)
{
    std::ifstream arquivo(path);
    Grafo g;
    if (!arquivo.is_open())
    {
        return g;
    }

    arquivo >> g.V;
    if (g.V == 0)
        return g;

    g.matriz.assign(g.V, std::vector<int>(g.V));
    for (int i = 0; i < g.V; ++i)
    {
        for (int j = 0; j < g.V; ++j)
        {
            if (!(arquivo >> g.matriz[i][j]))
            {
                std::cerr << "ERRO: Falha ao ler a matriz do arquivo " << path << " na posicao (" << i << "," << j << ")" << std::endl;
                g.V = 0;
                return g;
            }
            if (i != j && g.matriz[i][j] == 0)
                g.matriz[i][j] = INF;
        }
    }
    return g;
}

// --- Funções dos Algoritmos ---

std::vector<std::vector<int>> floyd_warshall(const Grafo &grafo)
{
    std::vector<std::vector<int>> dist(grafo.V, std::vector<int>(grafo.V));

    for (int i = 0; i < grafo.V; ++i)
    {
        for (int j = 0; j < grafo.V; ++j)
        {
            if (i == j)
                dist[i][j] = 0;
            else if (grafo.matriz[i][j] >= INF)
                dist[i][j] = INF;
            else
                dist[i][j] = grafo.matriz[i][j];
        }
    }

    for (int k = 0; k < grafo.V; ++k)
    {
        for (int i = 0; i < grafo.V; ++i)
        {
            for (int j = 0; j < grafo.V; ++j)
            {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    return dist;
}

int encontrar_min_dist(const std::vector<int> &dist, const std::vector<bool> &visitado, int V)
{
    int min_dist = INF;
    int min_index = -1;
    for (int v = 0; v < V; ++v)
    {
        if (!visitado[v] && dist[v] < min_dist)
        {
            min_dist = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

std::vector<int> dijkstra_simples(const Grafo &grafo, int origem)
{
    std::vector<int> dist(grafo.V, INF);
    std::vector<bool> visitado(grafo.V, false);
    dist[origem] = 0;

    for (int count = 0; count < grafo.V; ++count)
    {
        int u = encontrar_min_dist(dist, visitado, grafo.V);
        if (u == -1)
            break;
        visitado[u] = true;

        for (int v = 0; v < grafo.V; ++v)
        {
            if (!visitado[v] && grafo.matriz[u][v] < INF && dist[u] != INF && dist[u] + grafo.matriz[u][v] < dist[v])
            {
                dist[v] = dist[u] + grafo.matriz[u][v];
            }
        }
    }
    return dist;
}

std::vector<std::vector<int>> dijkstra_v_vezes(const Grafo &grafo)
{
    std::vector<std::vector<int>> todos_os_caminhos(grafo.V);
    for (int i = 0; i < grafo.V; ++i)
    {
        todos_os_caminhos[i] = dijkstra_simples(grafo, i);
    }
    return todos_os_caminhos;
}

// --- Funções de Análise ---

long long calcular_soma_distancias(const std::vector<std::vector<int>> &matriz, int V)
{
    long long soma_total = 0;
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            if (matriz[i][j] < INF)
            {
                soma_total += matriz[i][j];
            }
        }
    }
    return soma_total;
}

bool comparar_matrizes(const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B, int V)
{
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            if (A[i][j] != B[i][j])
                return false;
        }
    }
    return true;
}

// --- Função Principal ---

int main()
{
    std::vector<std::string> arquivos = {
        "Entrada 10.txt", "Entrada 25.txt", "Entrada 50.txt", "Entrada 75.txt",
        "Entrada 100.txt", "Entrada 150.txt", "Entrada 200.txt", "Entrada 250.txt",
        "Entrada 300.txt", "Entrada 400.txt", "Entrada 500.txt", "Entrada 650.txt",
        "Entrada 800.txt", "Entrada 1000.txt", "Entrada 1500.txt"};

    std::cout << "Iniciando calculo e validacao das distancias..." << std::endl;

    for (const auto &nome_arquivo : arquivos)
    {
        Grafo grafo = ler_grafo(nome_arquivo);
        if (grafo.V == 0)
        {
            continue; // Pula para o próximo arquivo se houver erro de leitura
        }

        std::cout << "\n========================================================" << std::endl;
        std::cout << "Arquivo: " << nome_arquivo << " (V = " << grafo.V << ")" << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;

        // Executa os algoritmos uma vez para obter as matrizes
        std::vector<std::vector<int>> matriz_floyd = floyd_warshall(grafo);
        std::vector<std::vector<int>> matriz_dijkstra = dijkstra_v_vezes(grafo);

        // Calcula a soma para cada matriz
        long long soma_floyd = calcular_soma_distancias(matriz_floyd, grafo.V);
        long long soma_dijkstra = calcular_soma_distancias(matriz_dijkstra, grafo.V);

        // Imprime o relatório de validação
        std::cout << "Soma das distancias (Floyd-Warshall): " << soma_floyd << std::endl;
        std::cout << "Soma das distancias (Dijkstra):     " << soma_dijkstra << std::endl;
        std::cout << "Validacao das matrizes: As matrizes sao " << (comparar_matrizes(matriz_floyd, matriz_dijkstra, grafo.V) ? "IGUAIS" : "DIFERENTES") << "\n";
    }

    std::cout << "\n========================================================" << std::endl;
    std::cout << "Validacao finalizada." << std::endl;

    return 0;
}