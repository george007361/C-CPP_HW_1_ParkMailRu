#include "utils.h"

#include <stdio.h>
#include <string.h>

long counter_simple(const char *array, const size_t len, const char *key) {
  if (!array) {
    fprintf(stderr, "Error counter_simple(): array ptr os null.\n");

    return 0;
  }
  if (!array) {
    fprintf(stderr, "Error counter_simple(): key ptr os null.\n");

    return 0;
  }
  unsigned long count = 0;
  size_t step = strlen(key);

  for (char *ptr = strstr(array, key); ptr && ptr - array < len;
       ptr = strstr(ptr + step, key)) {
    count++;
  }

  return count;
}