#ifndef CLI_H
#define CLI_H

#include "graph.h"
#include "db.h"
#include "algorithms.h"

// Estrutura que agrupa tudo o que a CLI precisa
typedef struct {
    Graph *graph;
    sqlite3 *db;
    Alias aliases[MAX_ALIASES];
    int alias_count;
} CLIContext;

// Função principal da CLI
void cli_run(CLIContext *ctx);

// Funções de cada menu
void cli_menu_nodes(CLIContext *ctx);
void cli_menu_edges(CLIContext *ctx);
void cli_menu_search(CLIContext *ctx);
void cli_menu_algorithms(CLIContext *ctx);
void cli_menu_stats(CLIContext *ctx);

// Funções auxiliares
void cli_print_menu(void);
EntityType cli_choose_entity_type(void);
RelationType cli_choose_relation_type(void);
void cli_print_separator(void);

#endif