#ifndef GRAPH_H
#define GRAPH_H

// Tamanhos máximos
#define MAX_NODES 100
#define MAX_EDGES 500
#define MAX_STR 100

// Tipos de entidades permitidos
typedef enum {
    THREAT_ACTOR,
    MALWARE,
    INDICATOR,
    CAMPAIGN,
    VULNERABILITY,
    IDENTITY,
    ATTACK_PATTERN
} EntityType;

// Tipos de relações permitidas
typedef enum {
    USES,
    INDICATES,
    TARGETS,
    EXPLOITS,
    ATTRIBUTED_TO,
    RELATED_TO
} RelationType;

// Estrutura de um nó
typedef struct {
    int id;
    EntityType type;
    char name[MAX_STR];
    char description[MAX_STR];
    int active; // 1 = existe, 0 = foi removido
} Node;

// Estrutura de uma relação
typedef struct {
    int id;
    int source_id;
    int target_id;
    RelationType type;
    int active;
} Edge;

// Estrutura principal do grafo
typedef struct {
    Node nodes[MAX_NODES];
    Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
} Graph;

// Funções disponíveis (declarações)
void init_graph(Graph *g);
int add_node(Graph *g, EntityType type, const char *name, const char *description);
int add_edge(Graph *g, int source_id, int target_id, RelationType type);
void list_neighbors(Graph *g, int node_id);
void print_node(Graph *g, int node_id);
void print_all_nodes(Graph *g);
const char* entity_type_to_string(EntityType type);
const char* relation_type_to_string(RelationType type);
int is_valid_relation(EntityType source, EntityType target, RelationType rel);

#endif