#include <stdio.h>
#include <string.h>
#include "graph.h"

// Converte EntityType para texto
const char* entity_type_to_string(EntityType type) {
    switch(type) {
        case THREAT_ACTOR:   return "threat-actor";
        case MALWARE:        return "malware";
        case INDICATOR:      return "indicator";
        case CAMPAIGN:       return "campaign";
        case VULNERABILITY:  return "vulnerability";
        case IDENTITY:       return "identity";
        case ATTACK_PATTERN: return "attack-pattern";
        default:             return "unknown";
    }
}

// Converte RelationType para texto
const char* relation_type_to_string(RelationType type) {
    switch(type) {
        case USES:          return "uses";
        case INDICATES:     return "indicates";
        case TARGETS:       return "targets";
        case EXPLOITS:      return "exploits";
        case ATTRIBUTED_TO: return "attributed-to";
        case RELATED_TO:    return "related-to";
        default:            return "unknown";
    }
}

// Valida se uma relação é permitida entre dois tipos de entidades
int is_valid_relation(EntityType source, EntityType target, RelationType rel) {
    // threat-actor uses malware
    if (source == THREAT_ACTOR && target == MALWARE && rel == USES) return 1;
    // threat-actor uses attack-pattern
    if (source == THREAT_ACTOR && target == ATTACK_PATTERN && rel == USES) return 1;
    // threat-actor targets identity
    if (source == THREAT_ACTOR && target == IDENTITY && rel == TARGETS) return 1;
    // threat-actor attributed-to campaign
    if (source == THREAT_ACTOR && target == CAMPAIGN && rel == ATTRIBUTED_TO) return 1;
    // malware uses attack-pattern
    if (source == MALWARE && target == ATTACK_PATTERN && rel == USES) return 1;
    // malware exploits vulnerability
    if (source == MALWARE && target == VULNERABILITY && rel == EXPLOITS) return 1;
    // indicator indicates malware
    if (source == INDICATOR && target == MALWARE && rel == INDICATES) return 1;
    // indicator indicates threat-actor
    if (source == INDICATOR && target == THREAT_ACTOR && rel == INDICATES) return 1;
    // campaign targets identity
    if (source == CAMPAIGN && target == IDENTITY && rel == TARGETS) return 1;
    // campaign uses malware
    if (source == CAMPAIGN && target == MALWARE && rel == USES) return 1;
    // qualquer entidade pode ter related-to
    if (rel == RELATED_TO) return 1;

    return 0; // relação não permitida
}

// Inicializa o grafo vazio
void init_graph(Graph *g) {
    g->node_count = 0;
    g->edge_count = 0;
    // marca todos como inactivos
    for (int i = 0; i < MAX_NODES; i++) g->nodes[i].active = 0;
    for (int i = 0; i < MAX_EDGES; i++) g->edges[i].active = 0;
}

// Adiciona um nó ao grafo
int add_node(Graph *g, EntityType type, const char *name, const char *description) {
    if (g->node_count >= MAX_NODES) {
        printf("ERRO: limite de nós atingido.\n");
        return -1;
    }
    int id = g->node_count;
    g->nodes[id].id = id;
    g->nodes[id].type = type;
    g->nodes[id].active = 1;
    strncpy(g->nodes[id].name, name, MAX_STR - 1);
    strncpy(g->nodes[id].description, description, MAX_STR - 1);
    g->node_count++;
    printf("Nó criado: [%d] %s (%s)\n", id, name, entity_type_to_string(type));
    return id;
}

// Adiciona uma relação entre dois nós
int add_edge(Graph *g, int source_id, int target_id, RelationType type) {
    if (g->edge_count >= MAX_EDGES) {
        printf("ERRO: limite de relações atingido.\n");
        return -1;
    }
    if (source_id < 0 || source_id >= g->node_count || !g->nodes[source_id].active) {
        printf("ERRO: nó origem inválido.\n");
        return -1;
    }
    if (target_id < 0 || target_id >= g->node_count || !g->nodes[target_id].active) {
        printf("ERRO: nó destino inválido.\n");
        return -1;
    }

    // Valida se a relação é permitida
    if (!is_valid_relation(g->nodes[source_id].type, g->nodes[target_id].type, type)) {
        printf("ERRO: relação '%s' entre '%s' e '%s' não é permitida.\n",
            relation_type_to_string(type),
            entity_type_to_string(g->nodes[source_id].type),
            entity_type_to_string(g->nodes[target_id].type));
        return -1;
    }

    int id = g->edge_count;
    g->edges[id].id = id;
    g->edges[id].source_id = source_id;
    g->edges[id].target_id = target_id;
    g->edges[id].type = type;
    g->edges[id].active = 1;
    g->edge_count++;

    printf("Relação criada: [%s] --%s--> [%s]\n",
        g->nodes[source_id].name,
        relation_type_to_string(type),
        g->nodes[target_id].name);
    return id;
}

// Imprime os detalhes de um nó
void print_node(Graph *g, int node_id) {
    if (node_id < 0 || node_id >= g->node_count || !g->nodes[node_id].active) {
        printf("ERRO: nó inexistente.\n");
        return;
    }
    Node *n = &g->nodes[node_id];
    printf("  ID: %d | Tipo: %s | Nome: %s | Desc: %s\n",
        n->id, entity_type_to_string(n->type), n->name, n->description);
}

// Lista todos os nós do grafo
void print_all_nodes(Graph *g) {
    printf("\n=== TODOS OS NOS ===\n");
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].active) print_node(g, i);
    }
}

// Lista os vizinhos de um nó (nós para onde tem relações)
void list_neighbors(Graph *g, int node_id) {
    if (node_id < 0 || node_id >= g->node_count || !g->nodes[node_id].active) {
        printf("ERRO: nó inexistente.\n");
        return;
    }
    printf("\n=== VIZINHOS DE [%s] ===\n", g->nodes[node_id].name);
    int found = 0;
    for (int i = 0; i < g->edge_count; i++) {
        if (g->edges[i].active && g->edges[i].source_id == node_id) {
            int target = g->edges[i].target_id;
            printf("  --%s--> [%s] (%s)\n",
                relation_type_to_string(g->edges[i].type),
                g->nodes[target].name,
                entity_type_to_string(g->nodes[target].type));
            found = 1;
        }
    }
    if (!found) printf("  Sem vizinhos.\n");
}