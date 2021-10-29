#ifndef DIGS_H
#define DIGS_H

#include <stdlib.h>
#define DIGRAPH_LEN 2

typedef struct Digraph {
  char key[DIGRAPH_LEN];
  unsigned long count;
} Digraph;

void interating_all(Digraph *digraphs, const int digraphs_count, char *array,
                    const size_t len);
long interating_part(const char *key, char *array, const size_t len);

#endif // !ALGS_H