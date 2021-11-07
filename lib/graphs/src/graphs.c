// / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#include "graphs.h"

#include <stdio.h>
#include <string.h>

Graph *create_graph() {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (!graph) {
    fprintf(stderr, "Can't acllocate mem for graph\n");

    return NULL;
  }

  graph->key = NULL;
  graph->count = 0;

  return graph;
}

Graph *set_graph_key(Graph *graph, const char *key) {
  if (!key) {
    fprintf(stderr, "Empty key\n");

    return NULL;
  }
  if (!graph) {
    fprintf(stderr, "Empty graph\n");

    return NULL;
  }

  size_t len = strlen(key);
  graph->key = (char *)malloc(sizeof(char) * len + 1);
  if (!graph) {
    fprintf(stderr, "Can't acllocate mem for graph's name\n");

    return NULL;
  }
  strcpy(graph->key, key);
  return graph;
}

Graph *set_graph_count(Graph *graph, const unsigned long count) {
  if (!graph) {
    fprintf(stderr, "Empty graph\n");

    return NULL;
  }

  graph->count = count;

  return graph;
}

Graph *set_graph(Graph *graph, const char *key, const unsigned long count) {
  if (!set_graph_key(graph, key)) return NULL;
  if (!set_graph_count(graph, count)) return NULL;

  return graph;
}

Graph *clear_graph(Graph *graph) {
  if (!graph) {
    fprintf(stderr, "Empy graph clear_degraph()\n");

    return NULL;
  }

  if (graph->key) free(graph->key);
  graph->key = NULL;

  graph->count = 0;

  return graph;
}

void free_graph(Graph *graph) {
  clear_graph(graph);
  free(graph);
}

void free_graphs(Graph ***graphs, size_t *graphs_count) {
  for (size_t i = 0; i < *graphs_count; i++) free_graph((*graphs)[i]);
  free(*graphs);
  *graphs_count = 0;
  *graphs = NULL;
}

int interprier(long *delta, Graph **graphs, const size_t graphs_count,
               const char *key_better, const char *key_worse) {
  unsigned long count_better = 0, count_worse = 0;
  if (!graphs) {
    fprintf(stderr, "Error interprier(): Empty graphs array \n");

    return EXIT_FAILURE;
  }
  if (!key_better || !key_worse) {
    fprintf(stderr, "Error interprier(): Empty key\n");

    return EXIT_FAILURE;
  }
  if (!delta) {
    fprintf(stderr, "Error interprier(): null ptr delta\n");

    return EXIT_FAILURE;
  }
  for (size_t i = 0; i < graphs_count; i++) {
    if (!strcmp(graphs[i]->key, key_better)) count_better = graphs[i]->count;
    if (!strcmp(graphs[i]->key, key_worse)) count_worse = graphs[i]->count;
  }
  *delta = count_better - count_worse;

  return EXIT_SUCCESS;
}
