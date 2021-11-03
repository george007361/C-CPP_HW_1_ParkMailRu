#include "parallel_identif.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum {
  EXIT_OK,
  EXIT_FAIL,
  EXIT_DIGRAPH_COUNTED_OK,
  EXIT_CANT_ALLOC_MEM,
  EXIT_CANT_FORK
};

int parse_using_digraph(unsigned long *count, const char *text,
                        const size_t text_len, const char *key) {
  long system_has_processors = sysconf(_SC_NPROCESSORS_ONLN);

  pid_t *part_pids = (pid_t *)malloc(system_has_processors * sizeof(pid_t));
  if (!part_pids) {
    fprintf(
        stderr,
        "Error parse_using_digraph(): cant allocate memory for pids array\n");
    return EXIT_CANT_ALLOC_MEM;
  }

  char *ptr = text;
  size_t step = text_len / system_has_processors + 1;
  for (size_t i = 0; i < system_has_processors; i++) {
    part_pids[i] = fork();
    if (part_pids[i] == -1) {
      printf(stderr,
             "Error parse_using_digraph(): Cant fork to separate text and "
             "count parallelly. %lu\n",
             i);
      return EXIT_CANT_FORK;
    }

    if (!part_pids[i]) {
      // Считаем количество данного диграфа в данном фрагменте длиной step
      *count += counter_simple(ptr, step, key);
      exit(EXIT_OK);
    }
    ptr += step;
  }

  for (size_t i = 0; i < system_has_processors; i++) {
    while (waitpid(part_pids[i], NULL, 0) > 0)
      ;
  }

  free(part_pids);
  return EXIT_OK;
}

int parse_text(const char *text, const size_t text_len, Digraph **digraphs,
               const size_t digraphs_count) {
  pid_t *digraph_pids = (pid_t *)malloc(digraphs_count * sizeof(pid_t));
  if (!digraph_pids) {
    fprintf(
        stderr,
        "Error parse_text(): cant allocate memory for digraphs pids array\n");
    return EXIT_CANT_ALLOC_MEM;
  }

  unsigned long *count_arr_shared =
      mmap(NULL, digraphs_count * sizeof(unsigned long), PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (!count_arr_shared) {
    fprintf(stderr,
            "Error: parse_text(): Cant allocate memory for  shared memory\n");
    return EXIT_CANT_ALLOC_MEM;
  }

  for (size_t i = 0; i < digraphs_count; i++) {
    digraph_pids[i] = fork();
    if (digraph_pids[i] == -1) {
      printf(
          stderr,
          "Error parse_text(): Cant fork to separate digraphs parsing. %lu\n",
          i);
      return EXIT_CANT_FORK;
    }

    if (!digraph_pids[i]) {
      // Считаем количество i-ого диграфа в тексте в отдельном процессе
      sleep(2);
      parse_using_digraph(&count_arr_shared[i], text, text_len,
                          digraphs[i]->key);
      exit(EXIT_DIGRAPH_COUNTED_OK);
    }
  }

  for (size_t i = 0; i < digraphs_count; i++) {
    while (waitpid(digraph_pids[i], NULL, 0) > 0)
      ;
  }
  for (size_t i = 0; i < digraphs_count; i++) {
    printf("%lu ", count_arr_shared[i]);
  }
  printf("ok! Enden\n");
  if (munmap(count_arr_shared, digraphs_count * sizeof(unsigned long))) {
    sprintf(stderr, "Error parse_text(): Cannot unmap mem\n");
    return EXIT_FAIL;
  }

  free(digraph_pids);
  return EXIT_OK;
}