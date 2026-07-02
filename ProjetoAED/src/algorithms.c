#include <stdio.h>
#include <string.h>
#include "algorithms.h"

// ============================================================
// BFS - Pesquisa em Largura
// ============================================================
void bfs(Graph *g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count || !g->nodes[start_id].active) {
        printf("ERRO: no de inicio invalido.\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    int queue[MAX_QUEUE];
    int front = 0, back = 0;

    printf("\n=== BFS A PARTIR DE [%s] ===\n", g->nodes[start_id].name);

    queue[back++] = start_id;
    visited[start_id] = 1;

    while (front < back) {
        int current = queue[front++];
        printf("  Visitar: [%s] (%s)\n",
            g->nodes[current].name,
            entity_type_to_string(g->nodes[current].type));

        for (int i = 0; i < g->edge_count; i++) {
            if (!g->edges[i].active) continue;
            if (g->edges[i].source_id != current) continue;

            int neighbor = g->edges[i].target_id;
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                queue[back++] = neighbor;
            }
        }
    }
}

// ============================================================
// DFS - Pesquisa em Profundidade (recursiva)
// ============================================================
static void dfs_recursive(Graph *g, int node_id, int *visited) {
    visited[node_id] = 1;
    printf("  Visitar: [%s] (%s)\n",
        g->nodes[node_id].name,
        entity_type_to_string(g->nodes[node_id].type));

    for (int i = 0; i < g->edge_count; i++) {
        if (!g->edges[i].active) continue;
        if (g->edges[i].source_id != node_id) continue;

        int neighbor = g->edges[i].target_id;
        if (!visited[neighbor]) {
            dfs_recursive(g, neighbor, visited);
        }
    }
}

void dfs(Graph *g, int start_id) {
    if (start_id < 0 || start_id >= g->node_count || !g->nodes[start_id].active) {
        printf("ERRO: no de inicio invalido.\n");
        return;
    }

    int visited[MAX_NODES] = {0};
    printf("\n=== DFS A PARTIR DE [%s] ===\n", g->nodes[start_id].name);
    dfs_recursive(g, start_id, visited);
}

// ============================================================
// Caminho mais curto (BFS)
// ============================================================
int shortest_path(Graph *g, int start_id, int end_id, int *path, int *path_len) {
    if (start_id < 0 || start_id >= g->node_count || !g->nodes[start_id].active ||
        end_id   < 0 || end_id   >= g->node_count || !g->nodes[end_id].active) {
        printf("ERRO: nos invalidos.\n");
        return 0;
    }

    int visited[MAX_NODES] = {0};
    int parent[MAX_NODES];
    int queue[MAX_QUEUE];
    int front = 0, back = 0;

    for (int i = 0; i < MAX_NODES; i++) parent[i] = -1;

    queue[back++] = start_id;
    visited[start_id] = 1;

    while (front < back) {
        int current = queue[front++];

        if (current == end_id) {
            // Reconstruir caminho
            *path_len = 0;
            int temp[MAX_PATH];
            int node = end_id;
            while (node != -1) {
                temp[(*path_len)++] = node;
                node = parent[node];
            }
            // Inverter
            for (int i = 0; i < *path_len; i++) {
                path[i] = temp[*path_len - 1 - i];
            }
            return 1;
        }

        for (int i = 0; i < g->edge_count; i++) {
            if (!g->edges[i].active) continue;
            if (g->edges[i].source_id != current) continue;

            int neighbor = g->edges[i].target_id;
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                parent[neighbor] = current;
                queue[back++] = neighbor;
            }
        }
    }

    return 0; // Caminho não encontrado
}

void print_path(Graph *g, int *path, int path_len) {
    printf("  Caminho encontrado (%d nos):\n", path_len);
    for (int i = 0; i < path_len; i++) {
        printf("  [%s] (%s)",
            g->nodes[path[i]].name,
            entity_type_to_string(g->nodes[path[i]].type));
        if (i < path_len - 1) printf(" --> ");
    }
    printf("\n");
}

// ============================================================
// PageRank simplificado
// ============================================================
void pagerank(Graph *g, float *scores, int iterations) {
    float damping = 0.85f;
    int n = g->node_count;

    // Inicializar scores iguais
    for (int i = 0; i < n; i++) scores[i] = 1.0f / n;

    for (int iter = 0; iter < iterations; iter++) {
        float new_scores[MAX_NODES] = {0};

        for (int i = 0; i < n; i++) {
            if (!g->nodes[i].active) continue;

            // Contar quantas saidas tem o no i
            int out_count = 0;
            for (int e = 0; e < g->edge_count; e++) {
                if (g->edges[e].active && g->edges[e].source_id == i) out_count++;
            }

            // Distribuir score pelos vizinhos
            if (out_count > 0) {
                float share = scores[i] / out_count;
                for (int e = 0; e < g->edge_count; e++) {
                    if (g->edges[e].active && g->edges[e].source_id == i) {
                        new_scores[g->edges[e].target_id] += share;
                    }
                }
            } else {
                // No sem saidas distribui igualmente
                for (int j = 0; j < n; j++) {
                    if (g->nodes[j].active) new_scores[j] += scores[i] / n;
                }
            }
        }

        // Aplicar damping factor
        for (int i = 0; i < n; i++) {
            scores[i] = (1.0f - damping) / n + damping * new_scores[i];
        }
    }
}

void print_pagerank(Graph *g, float *scores) {
    printf("\n=== RANKING DE ENTIDADES (PageRank) ===\n");

    // Ordenar por score (bubble sort simples)
    int order[MAX_NODES];
    for (int i = 0; i < g->node_count; i++) order[i] = i;

    for (int i = 0; i < g->node_count - 1; i++) {
        for (int j = i + 1; j < g->node_count; j++) {
            if (scores[order[j]] > scores[order[i]]) {
                int tmp = order[i];
                order[i] = order[j];
                order[j] = tmp;
            }
        }
    }

    for (int i = 0; i < g->node_count; i++) {
        int idx = order[i];
        if (!g->nodes[idx].active) continue;
        printf("  %d. [%s] (%s) -- Score: %.4f\n",
            i + 1,
            g->nodes[idx].name,
            entity_type_to_string(g->nodes[idx].type),
            scores[idx]);
    }
}

// ============================================================
// Resolução de entidades (aliases)
// ============================================================
void init_aliases(Alias *aliases, int *count) {
    *count = 0;
    // Aliases conhecidos
    strcpy(aliases[*count].name1, "APT29");
    strcpy(aliases[*count].name2, "Cozy Bear");
    (*count)++;

    strcpy(aliases[*count].name1, "APT28");
    strcpy(aliases[*count].name2, "Fancy Bear");
    (*count)++;

    strcpy(aliases[*count].name1, "APT1");
    strcpy(aliases[*count].name2, "Comment Crew");
    (*count)++;
}

int resolve_entity(Alias *aliases, int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(aliases[i].name1, name) == 0 ||
            strcmp(aliases[i].name2, name) == 0) {
            printf("  Resolucao: '%s' e '%s' sao o mesmo ator.\n",
                aliases[i].name1, aliases[i].name2);
            return 1;
        }
    }
    return 0;
}

// ============================================================
// Consultas analíticas
// ============================================================

// Que atores usam determinado malware?
void query_actors_using_malware(Graph *g, const char *malware_name) {
    printf("\n=== ATORES QUE USAM [%s] ===\n", malware_name);

    // Encontrar o id do malware
    int malware_id = -1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].active && g->nodes[i].type == MALWARE &&
            strcmp(g->nodes[i].name, malware_name) == 0) {
            malware_id = i;
            break;
        }
    }

    if (malware_id == -1) {
        printf("  Malware nao encontrado.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < g->edge_count; i++) {
        if (!g->edges[i].active) continue;
        if (g->edges[i].target_id == malware_id && g->edges[i].type == USES) {
            int src = g->edges[i].source_id;
            if (g->nodes[src].type == THREAT_ACTOR) {
                printf("  --> [%s]\n", g->nodes[src].name);
                found = 1;
            }
        }
    }
    if (!found) printf("  Nenhum ator encontrado.\n");
}

// Que indicadores apontam para determinada ameaça?
void query_indicators_for_threat(Graph *g, const char *threat_name) {
    printf("\n=== INDICADORES QUE APONTAM PARA [%s] ===\n", threat_name);

    int threat_id = -1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].active && strcmp(g->nodes[i].name, threat_name) == 0) {
            threat_id = i;
            break;
        }
    }

    if (threat_id == -1) {
        printf("  Ameaca nao encontrada.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < g->edge_count; i++) {
        if (!g->edges[i].active) continue;
        if (g->edges[i].target_id == threat_id && g->edges[i].type == INDICATES) {
            int src = g->edges[i].source_id;
            printf("  --> [%s] (%s)\n",
                g->nodes[src].name,
                entity_type_to_string(g->nodes[src].type));
            found = 1;
        }
    }
    if (!found) printf("  Nenhum indicador encontrado.\n");
}

// Caminho entre um indicador e um ator
void query_path_indicator_to_actor(Graph *g, int indicator_id, int actor_id) {
    printf("\n=== CAMINHO DE [%s] ATE [%s] ===\n",
        g->nodes[indicator_id].name,
        g->nodes[actor_id].name);

    int path[MAX_PATH];
    int path_len = 0;

    if (shortest_path(g, indicator_id, actor_id, path, &path_len)) {
        print_path(g, path, path_len);
    } else {
        printf("  Nenhum caminho encontrado.\n");
    }
}