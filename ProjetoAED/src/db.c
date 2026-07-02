#include <stdio.h>
#include <string.h>
#include "db.h"

// Abre a ligação à base de dados
int db_open(sqlite3 **db) {
    int rc = sqlite3_open(DB_NAME, db);
    if (rc != SQLITE_OK) {
        printf("ERRO ao abrir base de dados: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    printf("Base de dados aberta: %s\n", DB_NAME);
    return 1;
}

// Fecha a ligação à base de dados
void db_close(sqlite3 *db) {
    sqlite3_close(db);
    printf("Base de dados fechada.\n");
}

// Cria as tabelas se não existirem
int db_create_tables(sqlite3 *db) {
    const char *sql_nodes =
        "CREATE TABLE IF NOT EXISTS nodes ("
        "id INTEGER PRIMARY KEY,"
        "type INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "description TEXT"
        ");";

    const char *sql_edges =
        "CREATE TABLE IF NOT EXISTS edges ("
        "id INTEGER PRIMARY KEY,"
        "source_id INTEGER NOT NULL,"
        "target_id INTEGER NOT NULL,"
        "type INTEGER NOT NULL"
        ");";

    char *err = NULL;

    if (sqlite3_exec(db, sql_nodes, 0, 0, &err) != SQLITE_OK) {
        printf("ERRO ao criar tabela nodes: %s\n", err);
        sqlite3_free(err);
        return 0;
    }

    if (sqlite3_exec(db, sql_edges, 0, 0, &err) != SQLITE_OK) {
        printf("ERRO ao criar tabela edges: %s\n", err);
        sqlite3_free(err);
        return 0;
    }

    printf("Tabelas criadas com sucesso.\n");
    return 1;
}

// Guarda o grafo na base de dados
int db_save_graph(sqlite3 *db, Graph *g) {
    char sql[512];
    char *err = NULL;

    // Limpa tabelas antes de guardar
    sqlite3_exec(db, "DELETE FROM nodes;", 0, 0, NULL);
    sqlite3_exec(db, "DELETE FROM edges;", 0, 0, NULL);

    // Guarda os nos
    for (int i = 0; i < g->node_count; i++) {
        if (!g->nodes[i].active) continue;
        snprintf(sql, sizeof(sql),
            "INSERT INTO nodes (id, type, name, description) VALUES (%d, %d, '%s', '%s');",
            g->nodes[i].id,
            g->nodes[i].type,
            g->nodes[i].name,
            g->nodes[i].description);

        if (sqlite3_exec(db, sql, 0, 0, &err) != SQLITE_OK) {
            printf("ERRO ao guardar no %d: %s\n", i, err);
            sqlite3_free(err);
            return 0;
        }
    }

    // Guarda as relacoes
    for (int i = 0; i < g->edge_count; i++) {
        if (!g->edges[i].active) continue;
        snprintf(sql, sizeof(sql),
            "INSERT INTO edges (id, source_id, target_id, type) VALUES (%d, %d, %d, %d);",
            g->edges[i].id,
            g->edges[i].source_id,
            g->edges[i].target_id,
            g->edges[i].type);

        if (sqlite3_exec(db, sql, 0, 0, &err) != SQLITE_OK) {
            printf("ERRO ao guardar relacao %d: %s\n", i, err);
            sqlite3_free(err);
            return 0;
        }
    }

    printf("Grafo guardado na base de dados.\n");
    return 1;
}

// Carrega o grafo da base de dados
int db_load_graph(sqlite3 *db, Graph *g) {
    sqlite3_stmt *stmt;
    init_graph(g);

    // Carrega nos
    const char *sql_nodes = "SELECT id, type, name, description FROM nodes ORDER BY id;";
    if (sqlite3_prepare_v2(db, sql_nodes, -1, &stmt, NULL) != SQLITE_OK) {
        printf("ERRO ao preparar query de nos.\n");
        return 0;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id       = sqlite3_column_int(stmt, 0);
        int type     = sqlite3_column_int(stmt, 1);
        const char *name = (const char*)sqlite3_column_text(stmt, 2);
        const char *desc = (const char*)sqlite3_column_text(stmt, 3);

        g->nodes[id].id     = id;
        g->nodes[id].type   = (EntityType)type;
        g->nodes[id].active = 1;
        strncpy(g->nodes[id].name, name, MAX_STR - 1);
        strncpy(g->nodes[id].description, desc, MAX_STR - 1);
        g->node_count++;
    }
    sqlite3_finalize(stmt);

    // Carrega relacoes
    const char *sql_edges = "SELECT id, source_id, target_id, type FROM edges ORDER BY id;";
    if (sqlite3_prepare_v2(db, sql_edges, -1, &stmt, NULL) != SQLITE_OK) {
        printf("ERRO ao preparar query de relacoes.\n");
        return 0;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id        = sqlite3_column_int(stmt, 0);
        int source_id = sqlite3_column_int(stmt, 1);
        int target_id = sqlite3_column_int(stmt, 2);
        int type      = sqlite3_column_int(stmt, 3);

        g->edges[id].id        = id;
        g->edges[id].source_id = source_id;
        g->edges[id].target_id = target_id;
        g->edges[id].type      = (RelationType)type;
        g->edges[id].active    = 1;
        g->edge_count++;
    }
    sqlite3_finalize(stmt);

    printf("Grafo carregado da base de dados: %d nos, %d relacoes.\n",
        g->node_count, g->edge_count);
    return 1;
}

// Pesquisa nos por tipo
void db_search_nodes_by_type(sqlite3 *db, EntityType type) {
    sqlite3_stmt *stmt;
    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT id, name, description FROM nodes WHERE type = %d;", type);

    printf("\n=== NOS DO TIPO [%s] ===\n", entity_type_to_string(type));

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("ERRO na pesquisa.\n");
        return;
    }

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        const char *desc = (const char*)sqlite3_column_text(stmt, 2);
        printf("  [%d] %s - %s\n", id, name, desc);
        found = 1;
    }
    if (!found) printf("  Nenhum no encontrado.\n");
    sqlite3_finalize(stmt);
}

// Pesquisa relacoes por origem
void db_search_edges_by_source(sqlite3 *db, int source_id) {
    sqlite3_stmt *stmt;
    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT id, target_id, type FROM edges WHERE source_id = %d;", source_id);

    printf("\n=== RELACOES COM ORIGEM NO NO [%d] ===\n", source_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("ERRO na pesquisa.\n");
        return;
    }

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id        = sqlite3_column_int(stmt, 0);
        int target_id = sqlite3_column_int(stmt, 1);
        int type      = sqlite3_column_int(stmt, 2);
        printf("  Relacao [%d]: --%s--> no [%d]\n",
            id, relation_type_to_string((RelationType)type), target_id);
        found = 1;
    }
    if (!found) printf("  Nenhuma relacao encontrada.\n");
    sqlite3_finalize(stmt);
}

// Pesquisa relacoes por destino
void db_search_edges_by_target(sqlite3 *db, int target_id) {
    sqlite3_stmt *stmt;
    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT id, source_id, type FROM edges WHERE target_id = %d;", target_id);

    printf("\n=== RELACOES COM DESTINO NO NO [%d] ===\n", target_id);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("ERRO na pesquisa.\n");
        return;
    }

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id        = sqlite3_column_int(stmt, 0);
        int source_id = sqlite3_column_int(stmt, 1);
        int type      = sqlite3_column_int(stmt, 2);
        printf("  Relacao [%d]: no [%d] --%s-->\n",
            id, source_id, relation_type_to_string((RelationType)type));
        found = 1;
    }
    if (!found) printf("  Nenhuma relacao encontrada.\n");
    sqlite3_finalize(stmt);
}