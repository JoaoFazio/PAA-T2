#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <limits>
#include <filesystem> // Biblioteca para criar pastas (requer C++17)

// --- CONFIGURAÇÕES ---
const int INF = 999999;
const int LIMITE_IMPRESSAO_MATRIZ = 100;

// --- ESTRUTURAS E FUNÇÕES AUXILIARES ---
struct Grafo {
    int V = 0;
    std::vector<std::vector<int>> matriz;
};

Grafo ler_grafo(const std::string& path) {
    std::ifstream arquivo(path);
    Grafo g;
    if (!arquivo.is_open()) return g;
    arquivo >> g.V;
    if (g.V == 0) return g;
    g.matriz.assign(g.V, std::vector<int>(g.V));
    for (int i = 0; i < g.V; ++i) {
        for (int j = 0; j < g.V; ++j) {
            if (!(arquivo >> g.matriz[i][j])) {
                g.V = 0; return g;
            }
        }
    }
    return g;
}

void salvar_matriz_em_arquivo(const std::string& nome_arquivo, const std::vector<std::vector<int>>& matriz, int V) {
    std::ofstream out(nome_arquivo);
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            out << (matriz[i][j] >= INF ? -1 : matriz[i][j]) << (j == V - 1 ? "" : " ");
        }
        out << "\n";
    }
}

long long calcular_soma_distancias(const std::vector<std::vector<int>>& matriz, int V) {
    long long soma_total = 0;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (matriz[i][j] < INF) {
                soma_total += matriz[i][j];
            }
        }
    }
    return soma_total;
}

// --- ALGORITMOS ---

std::vector<std::vector<int>> floyd_warshall(const Grafo& grafo) {
    std::vector<std::vector<int>> dist = grafo.matriz;
    for (int i = 0; i < grafo.V; ++i) {
        for (int j = 0; j < grafo.V; ++j) {
            if (i == j) dist[i][j] = 0;
            else if (grafo.matriz[i][j] >= INF) dist[i][j] = INF;
        }
    }
    for (int k = 0; k < grafo.V; ++k) {
        for (int i = 0; i < grafo.V; ++i) {
            for (int j = 0; j < grafo.V; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    return dist;
}

int encontrar_min_dist(const std::vector<int>& dist, const std::vector<bool>& visitado, int V) {
    int min_dist = INF, min_index = -1;
    for (int v = 0; v < V; ++v) {
        if (!visitado[v] && dist[v] < min_dist) {
            min_dist = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

std::vector<int> dijkstra_simples(const Grafo& grafo, int origem) {
    std::vector<int> dist(grafo.V, INF);
    std::vector<bool> visitado(grafo.V, false);
    dist[origem] = 0;
    for (int count = 0; count < grafo.V; ++count) {
        int u = encontrar_min_dist(dist, visitado, grafo.V);
        if (u == -1) break;
        visitado[u] = true;
        for (int v = 0; v < grafo.V; ++v) {
            if (!visitado[v] && grafo.matriz[u][v] < INF && dist[u] != INF && dist[u] + grafo.matriz[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo.matriz[u][v];
            }
        }
    }
    return dist;
}

std::vector<std::vector<int>> dijkstra_v_vezes(const Grafo& grafo) {
    std::vector<std::vector<int>> todos_os_caminhos(grafo.V);
    for (int i = 0; i < grafo.V; ++i) {
        todos_os_caminhos[i] = dijkstra_simples(grafo, i);
    }
    return todos_os_caminhos;
}

// --- FUNÇÃO PRINCIPAL ---

int main() {
    std::vector<std::string> arquivos = {
        "Entrada 10.txt", "Entrada 25.txt", "Entrada 50.txt", "Entrada 75.txt",
        "Entrada 100.txt", "Entrada 150.txt", "Entrada 200.txt", "Entrada 250.txt",
        "Entrada 300.txt", "Entrada 400.txt", "Entrada 500.txt", "Entrada 650.txt",
        "Entrada 800.txt", "Entrada 1000.txt", "Entrada 1500.txt"
    };

    // --- CRIAÇÃO DE PASTAS E ARQUIVOS DE SAÍDA ---
    std::string dir_resultados = "Resultados";
    std::string dir_matrizes = dir_resultados + "/Matrizes";
    std::filesystem::create_directories(dir_matrizes);
    
    std::ofstream csv_file(dir_resultados + "/resultados_finais.csv");
    std::ofstream latex_file(dir_resultados + "/tabela_latex.txt");

    // Escreve os cabeçalhos
    csv_file << "Tamanho (V),Tempo Floyd (us),Tempo Dijkstra (us),Soma Floyd,Soma Dijkstra\n";
    latex_file << "% Copie o conteudo abaixo para sua tabela LaTeX\n"
               << "\\hline\n"
               << "\\textbf{Tamanho (V)} & \\textbf{Tempo (Floyd)} & \\textbf{Tempo (Dijkstra)} \\\\ \n"
               << "\\hline\n";

    std::cout << "Iniciando bateria de testes..." << std::endl;

    for (const auto& nome_arquivo : arquivos) {
        Grafo grafo = ler_grafo(nome_arquivo);
        if (grafo.V == 0) {
            std::cerr << "ERRO: Falha ao ler " << nome_arquivo << "\n";
            continue;
        }

        std::cout << "\nProcessando: " << nome_arquivo << " (V = " << grafo.V << ")" << std::endl;

        // Medição do Floyd-Warshall
        std::vector<double> tempos_floyd;
        for (int i = 0; i < 6; ++i) {
            auto ini = std::chrono::high_resolution_clock::now();
            floyd_warshall(grafo);
            auto fim = std::chrono::high_resolution_clock::now();
            if (i > 0) tempos_floyd.push_back(std::chrono::duration<double, std::micro>(fim - ini).count());
        }
        double media_floyd = std::accumulate(tempos_floyd.begin(), tempos_floyd.end(), 0.0) / tempos_floyd.size();
        
        // Medição do Dijkstra V vezes
        std::vector<double> tempos_dijkstra;
        for (int i = 0; i < 6; ++i) {
            auto ini = std::chrono::high_resolution_clock::now();
            dijkstra_v_vezes(grafo);
            auto fim = std::chrono::high_resolution_clock::now();
            if (i > 0) tempos_dijkstra.push_back(std::chrono::duration<double, std::micro>(fim - ini).count());
        }
        double media_dijkstra = std::accumulate(tempos_dijkstra.begin(), tempos_dijkstra.end(), 0.0) / tempos_dijkstra.size();

        // Obter resultados finais para validação
        std::vector<std::vector<int>> matriz_floyd = floyd_warshall(grafo);
        std::vector<std::vector<int>> matriz_dijkstra = dijkstra_v_vezes(grafo);
        long long soma_floyd = calcular_soma_distancias(matriz_floyd, grafo.V);
        long long soma_dijkstra = calcular_soma_distancias(matriz_dijkstra, grafo.V);

        // Exportar resultados
        csv_file << grafo.V << "," << media_floyd << "," << media_dijkstra << "," << soma_floyd << "," << soma_dijkstra << "\n";
        latex_file << grafo.V << " & " << std::fixed << std::setprecision(2) << media_floyd << " us & " 
                   << std::fixed << std::setprecision(2) << media_dijkstra << " us \\\\ \\hline\n";

        // Salvar matrizes condicionalmente
        if (grafo.V < LIMITE_IMPRESSAO_MATRIZ) {
            std::string nome_base = nome_arquivo.substr(0, nome_arquivo.find(".txt"));
            salvar_matriz_em_arquivo(dir_matrizes + "/" + nome_base + "_floyd.txt", matriz_floyd, grafo.V);
            salvar_matriz_em_arquivo(dir_matrizes + "/" + nome_base + "_dijkstra.txt", matriz_dijkstra, grafo.V);
        }
    }

    csv_file.close();
    latex_file.close();
    std::cout << "\n========================================================" << std::endl;
    std::cout << "Bateria de testes finalizada." << std::endl;
    std::cout << "Resultados salvos na pasta 'Resultados'." << std::endl;

    return 0;
}