#include <stdio.h>
#include <string.h>
#include "cli.h"

void cli_print_separator(void) {
    printf("\n========================================\n");
}

void cli_print_menu(void) {
    cli_print_separator();
    printf("   CYBER THREAT INTELLIGENCE - MENU\n");
    cli_print_separator();
    printf("  1. Gerir Nos\n");
    printf("  2. Gerir Relacoes\n");
    printf("  3. Pesquisar\n");
    printf("  4. Algoritmos de Analise\n");
    printf("  5. Estatisticas do Grafo\n");
    printf("  6. Guardar na Base de Dados\n");
    printf("  7. Carregar da Base de Dados\n");
    printf("  8. Demonstracao completa (Niveis 1-3)\n");
    printf("  0. Sair\n");
    cli_print_separator();
    printf("  Opcao: ");
}

EntityType cli_choose_entity_type(void) {
    printf("  Tipos disponiveis:\n");
    printf("    0 - threat-actor\n");
    printf("    1 - malware\n");
    printf("    2 - indicator\n");
    printf("    3 - campaign\n");
    printf("    4 - vulnerability\n");
    printf("    5 - identity\n");
    printf("    6 - attack-pattern\n");
    printf("  Escolha o tipo: ");
    int t;
    scanf("%d", &t);
    if (t < 0 || t > 6) t = 0;
    return (EntityType)t;
}

RelationType cli_choose_relation_type(void) {
    printf("  Tipos de relacao:\n");
    printf("    0 - uses\n");
    printf("    1 - indicates\n");
    printf("    2 - targets\n");
    printf("    3 - exploits\n");
    printf("    4 - attributed-to\n");
    printf("    5 - related-to\n");
    printf("  Escolha o tipo: ");
    int t;
    scanf("%d", &t);
    if (t < 0 || t > 5) t = 0;
    return (RelationType)t;
}

// ============================================================
// MENU - NODES
// ============================================================
void cli_menu_nodes(CLIContext *ctx) {
    int op;
    cli_print_separator();
    printf("   GERIR NOS\n");
    cli_print_separator();
    printf("  1. Adicionar no\n");
    printf("  2. Ver no por ID\n");
    printf("  3. Listar todos os nos\n");
    printf("  4. Ver vizinhos de um no\n");
    printf("  0. Voltar\n");
    printf("  Opcao: ");
    scanf("%d", &op);

    if (op == 1) {
        char name[MAX_STR], desc[MAX_STR];
        EntityType type = cli_choose_entity_type();
        printf("  Nome: ");
        scanf(" %[^\n]", name);
        printf("  Descricao: ");
        scanf(" %[^\n]", desc);
        add_node(ctx->graph, type, name, desc);
        db_save_graph(ctx->db, ctx->graph);
        printf("  No adicionado e guardado na base de dados.\n");

    } else if (op == 2) {
        int id;
        printf("  ID do no: ");
        scanf("%d", &id);
        print_node(ctx->graph, id);

    } else if (op == 3) {
        print_all_nodes(ctx->graph);

    } else if (op == 4) {
        int id;
        printf("  ID do no: ");
        scanf("%d", &id);
        list_neighbors(ctx->graph, id);
    }
}

// ============================================================
// MENU - EDGES
// ============================================================
void cli_menu_edges(CLIContext *ctx) {
    int op;
    cli_print_separator();
    printf("   GERIR RELACOES\n");
    cli_print_separator();
    printf("  1. Adicionar relacao\n");
    printf("  2. Pesquisar relacoes por origem\n");
    printf("  3. Pesquisar relacoes por destino\n");
    printf("  0. Voltar\n");
    printf("  Opcao: ");
    scanf("%d", &op);

    if (op == 1) {
        int src, dst;
        printf("  ID do no origem: ");
        scanf("%d", &src);
        printf("  ID do no destino: ");
        scanf("%d", &dst);
        RelationType type = cli_choose_relation_type();
        add_edge(ctx->graph, src, dst, type);
        db_save_graph(ctx->db, ctx->graph);
        printf("  Relacao adicionada e guardada na base de dados.\n");

    } else if (op == 2) {
        int id;
        printf("  ID do no origem: ");
        scanf("%d", &id);
        db_search_edges_by_source(ctx->db, id);

    } else if (op == 3) {
        int id;
        printf("  ID do no destino: ");
        scanf("%d", &id);
        db_search_edges_by_target(ctx->db, id);
    }
}

// ============================================================
// MENU - SEARCH
// ============================================================
void cli_menu_search(CLIContext *ctx) {
    int op;
    cli_print_separator();
    printf("   PESQUISAR\n");
    cli_print_separator();
    printf("  1. Nos por tipo\n");
    printf("  2. Atores que usam um malware\n");
    printf("  3. Indicadores de uma ameaca\n");
    printf("  4. Resolver alias de entidade\n");
    printf("  0. Voltar\n");
    printf("  Opcao: ");
    scanf("%d", &op);

    if (op == 1) {
        EntityType type = cli_choose_entity_type();
        db_search_nodes_by_type(ctx->db, type);

    } else if (op == 2) {
        char name[MAX_STR];
        printf("  Nome do malware: ");
        scanf(" %[^\n]", name);
        query_actors_using_malware(ctx->graph, name);

    } else if (op == 3) {
        char name[MAX_STR];
        printf("  Nome da ameaca: ");
        scanf(" %[^\n]", name);
        query_indicators_for_threat(ctx->graph, name);

    } else if (op == 4) {
        char name[MAX_STR];
        printf("  Nome da entidade: ");
        scanf(" %[^\n]", name);
        if (!resolve_entity(ctx->aliases, ctx->alias_count, name)) {
            printf("  Nenhum alias encontrado.\n");
        }
    }
}

// ============================================================
// MENU - ALGORITHMS
// ============================================================
void cli_menu_algorithms(CLIContext *ctx) {
    int op;
    cli_print_separator();
    printf("   ALGORITMOS DE ANALISE\n");
    cli_print_separator();
    printf("  1. BFS (pesquisa em largura)\n");
    printf("  2. DFS (pesquisa em profundidade)\n");
    printf("  3. Caminho mais curto entre dois nos\n");
    printf("  4. PageRank (ranking de entidades)\n");
    printf("  0. Voltar\n");
    printf("  Opcao: ");
    scanf("%d", &op);

    if (op == 1) {
        int id;
        printf("  ID do no de inicio: ");
        scanf("%d", &id);
        bfs(ctx->graph, id);

    } else if (op == 2) {
        int id;
        printf("  ID do no de inicio: ");
        scanf("%d", &id);
        dfs(ctx->graph, id);

    } else if (op == 3) {
        int src, dst;
        printf("  ID do no origem: ");
        scanf("%d", &src);
        printf("  ID do no destino: ");
        scanf("%d", &dst);
        int path[MAX_PATH];
        int path_len = 0;
        if (shortest_path(ctx->graph, src, dst, path, &path_len)) {
            print_path(ctx->graph, path, path_len);
        } else {
            printf("  Nenhum caminho encontrado.\n");
        }

    } else if (op == 4) {
        float scores[MAX_NODES] = {0};
        pagerank(ctx->graph, scores, 20);
        print_pagerank(ctx->graph, scores);
    }
}

// ============================================================
// MENU - STATS
// ============================================================
void cli_menu_stats(CLIContext *ctx) {
    cli_print_separator();
    printf("   ESTATISTICAS DO GRAFO\n");
    cli_print_separator();

    int counts[7] = {0};
    for (int i = 0; i < ctx->graph->node_count; i++) {
        if (ctx->graph->nodes[i].active) {
            counts[ctx->graph->nodes[i].type]++;
        }
    }

    printf("  Total de nos:      %d\n", ctx->graph->node_count);
    printf("  Total de relacoes: %d\n", ctx->graph->edge_count);
    printf("\n  Nos por tipo:\n");
    printf("    threat-actor:   %d\n", counts[0]);
    printf("    malware:        %d\n", counts[1]);
    printf("    indicator:      %d\n", counts[2]);
    printf("    campaign:       %d\n", counts[3]);
    printf("    vulnerability:  %d\n", counts[4]);
    printf("    identity:       %d\n", counts[5]);
    printf("    attack-pattern: %d\n", counts[6]);

    int n = ctx->graph->node_count;
    int e = ctx->graph->edge_count;
    float density = (n > 1) ? (float)e / (n * (n - 1)) : 0;
    printf("\n  Densidade do grafo: %.4f\n", density);
}

// ============================================================
// LOOP PRINCIPAL DA CLI
// ============================================================
void cli_run(CLIContext *ctx) {
    int op;
    do {
        cli_print_menu();
        scanf("%d", &op);

        switch (op) {
            case 1: cli_menu_nodes(ctx);      break;
            case 2: cli_menu_edges(ctx);      break;
            case 3: cli_menu_search(ctx);     break;
            case 4: cli_menu_algorithms(ctx); break;
            case 5: cli_menu_stats(ctx);      break;
            case 6:
                if (db_save_graph(ctx->db, ctx->graph))
                    printf("  Grafo guardado com sucesso.\n");
                break;
            case 7:
                if (db_load_graph(ctx->db, ctx->graph))
                    printf("  Grafo carregado com sucesso.\n");
                break;
            case 8:
                cli_print_separator();
                printf("   DEMONSTRACAO COMPLETA - NIVEIS 1, 2 e 3\n");
                cli_print_separator();

                printf("\n========== NIVEL 1: GRAFO BASICO ==========\n");
                print_all_nodes(ctx->graph);
                list_neighbors(ctx->graph, 0);
                list_neighbors(ctx->graph, 7);
                printf("\nTeste relacao invalida:\n");
                add_edge(ctx->graph, 4, 8, TARGETS);
                printf("\nTeste no inexistente:\n");
                print_node(ctx->graph, 99);
                printf("\nNIVEL 1 CONCLUIDO.\n");

                printf("\n========== NIVEL 2: PERSISTENCIA ==========\n");
                db_save_graph(ctx->db, ctx->graph);
                Graph g2;
                db_load_graph(ctx->db, &g2);
                printf("Nos originais: %d | Nos carregados: %d\n",
                    ctx->graph->node_count, g2.node_count);
                printf("Relacoes originais: %d | Relacoes carregadas: %d\n",
                    ctx->graph->edge_count, g2.edge_count);
                db_search_nodes_by_type(ctx->db, THREAT_ACTOR);
                db_search_nodes_by_type(ctx->db, MALWARE);
                db_search_edges_by_source(ctx->db, 0);
                db_search_edges_by_target(ctx->db, 2);
                printf("\nNIVEL 2 CONCLUIDO.\n");

                printf("\n========== NIVEL 3: ALGORITMOS ==========\n");
                bfs(ctx->graph, 0);
                dfs(ctx->graph, 0);

                int path[MAX_PATH];
                int path_len = 0;
                printf("\nCaminho de [192.168.10.5] ate [CVE-2021-44228]:\n");
                if (shortest_path(ctx->graph, 4, 6, path, &path_len))
                    print_path(ctx->graph, path, path_len);
                else
                    printf("  Nenhum caminho encontrado.\n");

                float scores[MAX_NODES] = {0};
                pagerank(ctx->graph, scores, 20);
                print_pagerank(ctx->graph, scores);

                printf("\nResolucao de entidades:\n");
                if (!resolve_entity(ctx->aliases, ctx->alias_count, "Cozy Bear"))
                    printf("  Nenhum alias encontrado.\n");
                if (!resolve_entity(ctx->aliases, ctx->alias_count, "APT28"))
                    printf("  Nenhum alias encontrado.\n");

                query_actors_using_malware(ctx->graph, "CozyMail");
                query_indicators_for_threat(ctx->graph, "CozyMail");
                printf("\nNIVEL 3 CONCLUIDO.\n");
                break;

            case 0:
                printf("\n  A sair... Ate logo!\n");
                break;
            default:
                printf("  Opcao invalida.\n");
        }
    } while (op != 0);
}