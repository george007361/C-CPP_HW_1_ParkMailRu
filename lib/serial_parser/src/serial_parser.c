// / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"
#include "utils.h"

enum {
  EXIT_OK,
  EXIT_FAIL,
  EXIT_GRAPH_COUNTED_OK,
  EXIT_NULL_TEXT,
  EXIT_NULL_GRAPHS,
  EXIT_NULL_KEY
};

int parse_text(const char *text, const size_t text_len, Graph **graphs,
               const size_t graphs_count) {
  // printf("Using serial parser...\n");

  if (!text) {
    fprintf(stderr, "Error: parse_text null ptr text\n");

    return EXIT_NULL_TEXT;
  }
  if (!graphs) {
    fprintf(stderr, "Error: parse_text null ptr graphs\n");

    return EXIT_NULL_GRAPHS;
  }

  for (size_t i = 0; i < graphs_count; i++) {
    if (!graphs[i]->key) {
      fprintf(stderr, "Error: parse_text null ptr key\n");

      return EXIT_NULL_KEY;
    }
    graphs[i]->count += counter_simple(text, text_len, graphs[i]->key);
  }

  return EXIT_OK;
}
