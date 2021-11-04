#include "digraphs.h"

#include <stdio.h>
#include <string.h>

Digraph *create_digraph() {
  Digraph *digraph = (Digraph *)malloc(sizeof(Digraph));
  if (!digraph) {
    fprintf(stderr, "Can't acllocate mem for digraph\n");

    return NULL;
  }

  digraph->key = NULL;
  digraph->count = 0;

  return digraph;
}

Digraph *set_digraph_key(Digraph *digraph, const char *key) {
  if (!key) {
    fprintf(stderr, "Empty key\n");

    return NULL;
  }
  if (!digraph) {
    fprintf(stderr, "Empty digraph\n");

    return NULL;
  }

  size_t len = strlen(key);
  digraph->key = (char *)malloc(sizeof(char) * len);
  memcpy(digraph->key, key, len);
  // digraph->key[len] = '\n';

  return digraph;
}

Digraph *set_digraph_count(Digraph *digraph, const unsigned long count) {
  if (!digraph) {
    fprintf(stderr, "Empty digraph\n");

    return NULL;
  }

  digraph->count = count;

  return digraph;
}

Digraph *set_digraph(Digraph *digraph, const char *key,
                     const unsigned long count) {
  if (!set_digraph_key(digraph, key)) return NULL;
  if (!set_digraph_count(digraph, count)) return NULL;

  return digraph;
}

Digraph *clear_digraph(Digraph *digraph) {
  if (!digraph) {
    fprintf(stderr, "Empy digraph clear_degraph()\n");
    
    return NULL;
  }

  if (digraph->key) free(digraph->key);
  digraph->count = 0;

  return digraph;
}

void free_digraph(Digraph *digraph) {
  clear_digraph(digraph);
  free(digraph);
}