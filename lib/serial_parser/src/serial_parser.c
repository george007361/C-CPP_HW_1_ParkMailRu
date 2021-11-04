#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"
#include "utils.h"

enum { EXIT_OK, EXIT_FAIL, EXIT_GRAPH_COUNTED_OK };

int parse_text(const char *text, const size_t text_len, Graph **graphs,
               const size_t graphs_count) {
  // printf("Using serial parser...\n");

  for (size_t i = 0; i < graphs_count; i++) {
    graphs[i]->count += counter_simple(text, text_len, graphs[i]->key);
  }

  return EXIT_OK;
}