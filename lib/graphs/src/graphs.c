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
  graph->key = (char *)malloc(sizeof(char) * len);
  memcpy(graph->key, key, len);
  // graph->key[len] = '\n';

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

Graph *set_graph(Graph *graph, const char *key,
                     const unsigned long count) {
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
  graph->count = 0;

  return graph;
}

void free_graph(Graph *graph) {
  clear_graph(graph);
  free(graph);
}