#include "serial_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

enum {
  EXIT_OK,
  EXIT_FAIL,
  EXIT_DIGRAPH_COUNTED_OK,
  EXIT_CANT_ALLOC_MEM,
  EXIT_CANT_FORK
};

int parse_text(const char *text, const size_t text_len, Digraph **digraphs,
               const size_t digraphs_count) {
  for (size_t i = 0; i < digraphs_count; i++) {
    digraphs[i]->count += counter_simple(text, text_len, digraphs[i]->key);
  }

  return EXIT_OK;
}