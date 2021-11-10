//  / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#ifndef LIB_GRAPHS_INCLUDE_GRAPHS_H_
#define LIB_GRAPHS_INCLUDE_GRAPHS_H_

#include <stdlib.h>

typedef struct Graph {
  char *key;
  unsigned long count;
} Graph;

Graph *create_graph();
Graph *set_graph(Graph *graph, const char *key, const unsigned long count);
Graph *set_graph_key(Graph *graph, const char *key);
Graph *set_graph_count(Graph *graph, const unsigned long count);
Graph *clear_graph(Graph *graph);
void free_graph(Graph *graph);
void free_graphs(Graph ***graphs, size_t *graphs_count);
int interprier(long *delta, Graph **graphs, const size_t graphs_count,
               const char *key_better, const char *key_worse);
#endif  // LIB_GRAPHS_INCLUDE_GRAPHS_H_
