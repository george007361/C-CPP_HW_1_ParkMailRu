#include "digraphs.h"

#include <stdio.h>
#include <string.h>

void interating_all(Digraph *digraphs, const int digraphs_count, char *array,
                     const size_t len) {
  for (size_t i = 0; i < digraphs_count; i++) {
    digraphs[i].count = interating_part(digraphs[i].key, array, len);
  }
}


long interating_part(const char *key, char *array, const size_t len)
{
    size_t step = strlen(key);
    long count = 0;
    for (char *p = array;
         (p = strstr(p, key)) + step - array < len;) {
      count++;
      p += step;
    }
    return count;
}
