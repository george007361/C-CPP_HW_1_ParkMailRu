#include "utils.h"

#include <string.h>

unsigned long counter_simple(const char *array, const size_t len,
                             const char *key) {
  unsigned long count = 0;
  size_t step = strlen(key);

  for (char *ptr = strstr(array, key); ptr && ptr - array < len;
       ptr = strstr(ptr + step, key)) {
    count++;
  }

  return count;
}