import numpy as np
import matplotlib.pyplot as plt
import os

def ler_matriz(arquivo):
    with open(arquivo, 'r') as f:
        linhas = f.readlines()
    return np.array([[int(x) if x != "-1" else np.inf for x in linha.strip().split()] for linha in linhas])

def desenhar_matriz(ax, matriz, titulo):
    im = ax.imshow(matriz, cmap='viridis', interpolation='nearest')
    ax.set_title(titulo)
    for i in range(matriz.shape[0]):
        for j in range(matriz.shape[1]):
            valor = matriz[i, j]
            texto = "INF" if np.isinf(valor) else str(int(valor))
            ax.text(j, i, texto, ha="center", va="center",
                    color="white" if valor < np.max(matriz)/2 else "black", fontsize=6)
    ax.set_xticks([])
    ax.set_yticks([])
    return im

# Crie a pasta de saída se não existir
os.makedirs("comparacoes", exist_ok=True)

# Lista de tamanhos que você mencionou
tamanhos = [10, 25, 50, 75, 100, 150, 200, 250, 300, 400, 500, 650, 800, 1000, 1500]

for tamanho in tamanhos:
    base = f"Entrada {tamanho}"
    arquivo_floyd = f"{base}_floyd.txt"
    arquivo_dijkstra = f"{base}_dijkstra.txt"

    # Verifica se os dois arquivos existem
    if not (os.path.exists(arquivo_floyd) and os.path.exists(arquivo_dijkstra)):
        print(f"[!] Pulando {base}: arquivos não encontrados")
        continue

    floyd = ler_matriz(arquivo_floyd)
    dijkstra = ler_matriz(arquivo_dijkstra)

    fig, axs = plt.subplots(1, 2, figsize=(12, 6))
    desenhar_matriz(axs[0], floyd, "Floyd-Warshall")
    desenhar_matriz(axs[1], dijkstra, "Dijkstra V Vezes")
    plt.tight_layout()

    # Salva na pasta "comparacoes"
    caminho_saida = f"comparacoes/{base}.png"
    plt.savefig(caminho_saida, dpi=300)
    plt.close()
    print(f"[✓] Imagem salva: {caminho_saida}")
