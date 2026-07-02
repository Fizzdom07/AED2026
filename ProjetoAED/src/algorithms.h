#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h"

// Tamanho máximo para filas e pilhas dos algoritmos
#define MAX_QUEUE 100
#define MAX_PATH  100

// Aliases para resolução de entidades
typedef struct {
    char name1[MAX_STR];
    char name2[MAX_STR];
} Alias;

#define MAX_ALIASES 20

// BFS - Pesquisa em largura a partir de um nó
void bfs(Graph *g, int start_id);

// DFS - Pesquisa em profundidade a partir de um nó
void dfs(Graph *g, int start_id);

// Caminho mais curto entre dois nós (usando BFS)
int shortest_path(Graph *g, int start_id, int end_id, int *path, int *path_len);

// Imprime o caminho encontrado
void print_path(Graph *g, int *path, int path_len);

// PageRank simplificado
void pagerank(Graph *g, float *scores, int iterations);

// Imprime o ranking das entidades
void print_pagerank(Graph *g, float *scores);

// Resolução de entidades (aliases)
void init_aliases(Alias *aliases, int *count);
int resolve_entity(Alias *aliases, int count, const char *name);

// Consultas analíticas
void query_actors_using_malware(Graph *g, const char *malware_name);
void query_indicators_for_threat(Graph *g, const char *threat_name);
void query_path_indicator_to_actor(Graph *g, int indicator_id, int actor_id);

#endif