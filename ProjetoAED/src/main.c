#include <stdio.h>
#include "graph.h"
#include "db.h"
#include "algorithms.h"
#include "cli.h"

// ============================================================
// DADOS SINTETICOS
// ============================================================
void create_synthetic_data(Graph *g) {
    printf("\n========================================\n");
    printf("       A CRIAR DADOS SINTETICOS\n");
    printf("========================================\n\n");

    int apt29      = add_node(g, THREAT_ACTOR,   "APT29",           "Grupo russo tambem conhecido como Cozy Bear");
    int apt28      = add_node(g, THREAT_ACTOR,   "APT28",           "Grupo russo tambem conhecido como Fancy Bear");
    int cozymail   = add_node(g, MALWARE,        "CozyMail",        "Malware de espionagem usado pelo APT29");
    int xagent     = add_node(g, MALWARE,        "X-Agent",         "Malware modular usado pelo APT28");
    int ip_susp    = add_node(g, INDICATOR,      "192.168.10.5",    "IP suspeito associado a comunicacoes C2");
    int hash_mal   = add_node(g, INDICATOR,      "a1b2c3d4e5f6",    "Hash de ficheiro malicioso identificado");
    int cve_2021   = add_node(g, VULNERABILITY,  "CVE-2021-44228",  "Log4Shell - vulnerabilidade critica");
    int op_ghost   = add_node(g, CAMPAIGN,       "Operation Ghost", "Campanha de espionagem atribuida ao APT29");
    int nato       = add_node(g, IDENTITY,       "NATO",            "Organizacao alvo de campanhas de espionagem");
    int spearphish = add_node(g, ATTACK_PATTERN, "Spear Phishing",  "Tecnica de phishing dirigido");

    printf("\n========================================\n");
    printf("       A CRIAR RELACOES\n");
    printf("========================================\n\n");

    add_edge(g, apt29,    cozymail,   USES);
    add_edge(g, apt28,    xagent,     USES);
    add_edge(g, apt29,    spearphish, USES);
    add_edge(g, apt28,    spearphish, USES);
    add_edge(g, apt29,    op_ghost,   ATTRIBUTED_TO);
    add_edge(g, apt29,    nato,       TARGETS);
    add_edge(g, op_ghost, nato,       TARGETS);
    add_edge(g, op_ghost, cozymail,   USES);
    add_edge(g, ip_susp,  cozymail,   INDICATES);
    add_edge(g, hash_mal, xagent,     INDICATES);
    add_edge(g, cozymail, cve_2021,   EXPLOITS);
    add_edge(g, xagent,   spearphish, USES);
}

// ============================================================
// NIVEL 1 - Testes basicos
// ============================================================
void run_nivel1(Graph *g) {
    printf("\n========================================\n");
    printf("         NIVEL 1 - GRAFO BASICO\n");
    printf("========================================\n");

    print_all_nodes(g);
    list_neighbors(g, 0);
    list_neighbors(g, 7);

    printf("\n--- Teste relacao invalida ---\n");
    add_edge(g, 4, 8, TARGETS);

    printf("\n--- Teste no inexistente ---\n");
    print_node(g, 99);

    printf("\nNIVEL 1 CONCLUIDO.\n");
}

// ============================================================
// NIVEL 2 - Testes de persistencia
// ============================================================
void run_nivel2(sqlite3 *db, Graph *g) {
    printf("\n========================================\n");
    printf("       NIVEL 2 - PERSISTENCIA\n");
    printf("========================================\n");

    printf("\n--- Teste 1: Guardar grafo na BD ---\n");
    if (db_save_graph(db, g)) printf("OK: Grafo guardado.\n");

    printf("\n--- Teste 2: Carregar grafo da BD ---\n");
    Graph g2;
    if (db_load_graph(db, &g2)) {
        printf("OK: Grafo carregado.\n");
        printf("    Nos: %d | Relacoes: %d\n", g2.node_count, g2.edge_count);
    }

    printf("\n--- Teste 3: Integridade ---\n");
    int ok = 1;
    for (int i = 0; i < g->node_count; i++) {
        if (g->nodes[i].type != g2.nodes[i].type) { ok = 0; break; }
    }
    printf(ok ? "OK: Estrutura preservada.\n" : "ERRO: Estrutura diferente!\n");

    printf("\n--- Teste 4: Pesquisa por tipo ---\n");
    db_search_nodes_by_type(db, THREAT_ACTOR);
    db_search_nodes_by_type(db, MALWARE);

    printf("\n--- Teste 5: Pesquisa de relacoes ---\n");
    db_search_edges_by_source(db, 0);
    db_search_edges_by_target(db, 2);

    printf("\nNIVEL 2 CONCLUIDO.\n");
}

// ============================================================
// NIVEL 3 - Algoritmos de analise
// ============================================================
void run_nivel3(Graph *g) {
    printf("\n========================================\n");
    printf("      NIVEL 3 - ALGORITMOS\n");
    printf("========================================\n");

    printf("\n--- Teste 1: BFS ---\n");
    bfs(g, 0);

    printf("\n--- Teste 2: DFS ---\n");
    dfs(g, 0);

    printf("\n--- Teste 3: Caminho mais curto ---\n");
    int path[MAX_PATH];
    int path_len = 0;

    printf("De [192.168.10.5] ate [CVE-2021-44228]:\n");
    if (shortest_path(g, 4, 6, path, &path_len)) {
        print_path(g, path, path_len);
    } else {
        printf("  Nenhum caminho encontrado.\n");
    }

    printf("\nDe [APT28] ate [NATO]:\n");
    if (shortest_path(g, 1, 8, path, &path_len)) {
        print_path(g, path, path_len);
    } else {
        printf("  Nenhum caminho encontrado.\n");
    }

    printf("\n--- Teste 4: PageRank ---\n");
    float scores[MAX_NODES] = {0};
    pagerank(g, scores, 20);
    print_pagerank(g, scores);

    printf("\n--- Teste 5: Resolucao de entidades ---\n");
    Alias aliases[MAX_ALIASES];
    int alias_count = 0;
    init_aliases(aliases, &alias_count);

    printf("A pesquisar 'Cozy Bear':\n");
    if (!resolve_entity(aliases, alias_count, "Cozy Bear"))
        printf("  Nenhum alias encontrado.\n");

    printf("A pesquisar 'APT28':\n");
    if (!resolve_entity(aliases, alias_count, "APT28"))
        printf("  Nenhum alias encontrado.\n");

    printf("A pesquisar 'Unknown':\n");
    if (!resolve_entity(aliases, alias_count, "Unknown"))
        printf("  Nenhum alias encontrado.\n");

    printf("\n--- Teste 6: Consultas analiticas ---\n");
    query_actors_using_malware(g, "CozyMail");
    query_actors_using_malware(g, "X-Agent");
    query_indicators_for_threat(g, "CozyMail");
    query_path_indicator_to_actor(g, 4, 0);

    printf("\nNIVEL 3 CONCLUIDO.\n");
}

// ============================================================
// MAIN
// ============================================================
int main() {
    // Inicializar grafo
    Graph g;
    init_graph(&g);

    // Abrir base de dados
    sqlite3 *db;
    if (!db_open(&db)) return 1;
    if (!db_create_tables(db)) { db_close(db); return 1; }

    // Tentar carregar da base de dados
    printf("A tentar carregar grafo da base de dados...\n");
    db_load_graph(db, &g);

    // Se nao havia nada na BD, criar dados sinteticos
    if (g.node_count == 0) {
        printf("Base de dados vazia. A criar dados sinteticos...\n");
        create_synthetic_data(&g);
        db_save_graph(db, &g);
        run_nivel1(&g);
        run_nivel2(db, &g);
        run_nivel3(&g);
    } else {
        printf("Grafo carregado: %d nos, %d relacoes.\n", g.node_count, g.edge_count);
    }

    // NIVEL 4 - CLI interativa
    printf("\n========================================\n");
    printf("      NIVEL 4 - CLI INTERATIVA\n");
    printf("========================================\n");
    printf("  A iniciar interface interativa...\n");

    CLIContext ctx;
    ctx.graph = &g;
    ctx.db = db;
    init_aliases(ctx.aliases, &ctx.alias_count);

    cli_run(&ctx);

    // Fechar base de dados
    db_close(db);

    return 0;
}