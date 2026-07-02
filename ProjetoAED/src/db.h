#ifndef DB_H
#define DB_H

#include "graph.h"
#include "sqlite3.h"

// Nome da base de dados
#define DB_NAME "threat_intel.db"

// Funções disponíveis
int  db_open(sqlite3 **db);
void db_close(sqlite3 *db);
int  db_create_tables(sqlite3 *db);
int  db_save_graph(sqlite3 *db, Graph *g);
int  db_load_graph(sqlite3 *db, Graph *g);
void db_search_nodes_by_type(sqlite3 *db, EntityType type);
void db_search_edges_by_source(sqlite3 *db, int source_id);
void db_search_edges_by_target(sqlite3 *db, int target_id);

#endif