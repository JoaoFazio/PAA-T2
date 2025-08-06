import matplotlib.pyplot as plt
import numpy as np

# Dados da sua tabela final
tamanho_v = [
    10, 25, 50, 75, 100, 150, 200, 250, 300, 400, 500, 650, 800, 1000, 1500
]

tempo_dijkstra_us = [
    0, 0.4006, 3.3026, 10.5214, 25.0946, 81.6510, 191.6216, 373.5242,
    638.5452, 1531.9150, 2985.4488, 7161.0092, 12799.2906, 28501.8588,
    91995.2446
]
tempo_floyd_us = [
    0, 0.3008, 1.8296, 5.8104, 14.2800, 46.6170, 109.1382, 216.2846,
    368.3520, 866.6526, 1705.5662, 4185.4066, 7508.0016, 15043.9142,
    50308.0356
]


# --- Criação e Formatação do Gráfico ---
plt.figure(figsize=(12, 7))

# Plotar os dados
plt.plot(tamanho_v, tempo_floyd_us, marker='o', linestyle='-', label='Floyd-Warshall')
plt.plot(tamanho_v, tempo_dijkstra_us, marker='o', linestyle='-', label='Dijkstra V x Vezes')

# Usar escala logarítmica no eixo Y para melhor visualização
plt.yscale('log')

# Rótulos dos eixos
plt.xlabel('Tamanho da Entrada (Número de Vértices - V)')
plt.ylabel('Tempo Médio de Execução (microssegundos - µs)')

# Legenda e Grade
plt.legend()
plt.grid(True, which="both", ls="--", linewidth=0.5)

# Salvar a imagem do gráfico
plt.savefig('grafico_final.png', dpi=300, bbox_inches='tight')

print("Gráfico 'grafico_final.png' foi salvo com sucesso!")