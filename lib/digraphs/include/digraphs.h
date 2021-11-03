#ifndef DIGS_H
#define DIGS_H

#include <stdlib.h>
#define DIGRAPH_LEN 2

typedef struct Digraph {
  char *key;
  unsigned long count;
} Digraph;

// void init_digraph();
Digraph *create_digraph();
Digraph *set_digraph(Digraph *digraph, const char *key,
                     const unsigned long count);
Digraph *set_digraph_key(Digraph *digraph, const char *key);
Digraph *set_digraph_count(Digraph *digraph, const unsigned long count);
Digraph *clear_digraph(Digraph *digraph);
void free_digraph(Digraph *digraph);

unsigned long counter_simple(const char *array, const size_t len,
                             const char *key);
#endif  // !ALGS_H