#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <numeric>
#include <iomanip>

const int INF = 999999;

// Função para ler o grafo
std::vector<std::vector<int>> ler_grafo(const std::string &path, int &V)
{
    std::ifstream arquivo(path);
    if (!arquivo.is_open())
    {
        V = 0;
        return {};
    }

    arquivo >> V;
    if (V == 0)
        return {};

    std::vector<std::vector<int>> grafo(V, std::vector<int>(V));
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            if (!(arquivo >> grafo[i][j]))
            {
                std::cerr << "Erro ao ler a matriz (" << i << ", " << j << ")\n";
                V = 0;
                return {};
            }
            if (i != j && grafo[i][j] == 0)
            {
                grafo[i][j] = INF;
            }
        }
    }
    return grafo;
}

// Função para imprimir matriz
void imprimir_matriz(const std::vector<std::vector<int>> &matriz, int V)
{
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            if (matriz[i][j] >= INF)
                std::cout << std::setw(7) << "INF";
            else
                std::cout << std::setw(7) << matriz[i][j];
        }
        std::cout << "\n";
    }
}

// Função para salvar matriz em arquivo
void salvar_matriz_em_arquivo(const std::string &nome, const std::vector<std::vector<int>> &matriz, int V)
{
    std::ofstream out(nome);
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            out << (matriz[i][j] >= INF ? -1 : matriz[i][j]) << (j == V - 1 ? "" : " ");
        }
        out << "\n";
    }
}

// Comparar duas matrizes
bool comparar_matrizes(const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B, int V)
{
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            if (A[i][j] != B[i][j])
                return false; // se achar algo diferente, retorna falso
    return true;              // se todas as entradas forem iguais, retorna verdadeiro
}

// Floyd-Warshall
std::vector<std::vector<int>> floyd_warshall(const std::vector<std::vector<int>> &grafo, int V)
{
    std::vector<std::vector<int>> dist = grafo;
    for (int k = 0; k < V; ++k)
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                if (dist[i][k] < INF && dist[k][j] < INF && dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
    return dist;
}

// Dijkstra
int encontrar_min_dist(const std::vector<int> &dist, const std::vector<bool> &visitado, int V)
{
    int min_dist = INF, min_index = -1;
    for (int v = 0; v < V; ++v)
        if (!visitado[v] && dist[v] < min_dist)
            min_dist = dist[v], min_index = v;
    return min_index;
}

std::vector<int> dijkstra_simples(const std::vector<std::vector<int>> &grafo, int V, int origem)
{
    std::vector<int> dist(V, INF);
    std::vector<bool> visitado(V, false);
    dist[origem] = 0;

    for (int count = 0; count < V; ++count)
    {
        int u = encontrar_min_dist(dist, visitado, V);
        if (u == -1)
            break;
        visitado[u] = true;

        for (int v = 0; v < V; ++v)
            if (!visitado[v] && grafo[u][v] != INF && dist[u] + grafo[u][v] < dist[v])
                dist[v] = dist[u] + grafo[u][v];
    }
    return dist;
}

std::vector<std::vector<int>> dijkstra_v_vezes(const std::vector<std::vector<int>> &grafo, int V)
{
    std::vector<std::vector<int>> caminhos(V);
    for (int i = 0; i < V; ++i)
        caminhos[i] = dijkstra_simples(grafo, V, i);
    return caminhos;
}

int main()
{
    std::vector<std::string> arquivos = {
        "Entrada 1500.txt" // Só mudar o número do arquivo aqui
    };

    for (const auto &nome_arquivo : arquivos)
    {
        int V;
        std::vector<std::vector<int>> grafo = ler_grafo(nome_arquivo, V);

        if (V == 0)
        {
            std::cerr << "ERRO: Falha ao ler " << nome_arquivo << "\n";
            continue;
        }

        std::cout << "\n==========================================\n";
        std::cout << "Arquivo: " << nome_arquivo << " (V = " << V << ")\n";

        // Floyd
        std::vector<double> tempos_floyd;
        std::vector<std::vector<int>> matriz_floyd;
        for (int i = 0; i < 6; ++i)
        {
            auto ini = std::chrono::high_resolution_clock::now();
            matriz_floyd = floyd_warshall(grafo, V);
            auto fim = std::chrono::high_resolution_clock::now();
            if (i > 0)
            {
                std::chrono::duration<double, std::micro> duracao = fim - ini;
                tempos_floyd.push_back(duracao.count());
            }
        }

        // Dijkstra
        std::vector<double> tempos_dijkstra;
        std::vector<std::vector<int>> matriz_dijkstra;
        for (int i = 0; i < 6; ++i)
        {
            auto ini = std::chrono::high_resolution_clock::now();
            matriz_dijkstra = dijkstra_v_vezes(grafo, V);
            auto fim = std::chrono::high_resolution_clock::now();
            if (i > 0)
            {
                std::chrono::duration<double, std::micro> duracao = fim - ini;
                tempos_dijkstra.push_back(duracao.count());
            }
        }

        double media_floyd = std::accumulate(tempos_floyd.begin(), tempos_floyd.end(), 0.0) / tempos_floyd.size();
        double media_dijkstra = std::accumulate(tempos_dijkstra.begin(), tempos_dijkstra.end(), 0.0) / tempos_dijkstra.size();

        std::cout << "Tempo medio Floyd-Warshall: " << std::fixed << std::setprecision(4) << media_floyd << " us\n";
        std::cout << "Tempo medio Dijkstra V vezes: " << std::fixed << std::setprecision(4) << media_dijkstra << " us\n";

        // Comparação
        bool iguais = comparar_matrizes(matriz_floyd, matriz_dijkstra, V);
        std::cout << "As matrizes sao " << (iguais ? "IGUAIS" : "DIFERENTES") << "\n";

        if (V <= 25)
        {
            std::cout << "\n--- Matriz Floyd-Warshall ---\n";
            imprimir_matriz(matriz_floyd, V);
            std::cout << "\n--- Matriz Dijkstra V vezes ---\n";
            imprimir_matriz(matriz_dijkstra, V);
        }

        // Salvar as matrizes em arquivos para o Python
        std::string nome_base = nome_arquivo.substr(0, nome_arquivo.find(".txt"));
        salvar_matriz_em_arquivo(nome_base + "_floyd.txt", matriz_floyd, V);
        salvar_matriz_em_arquivo(nome_base + "_dijkstra.txt", matriz_dijkstra, V);
    }

    return 0;
}
