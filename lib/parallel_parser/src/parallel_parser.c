//  / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"
#include "utils.h"

#define PART_MIN_LEN_BYTES ((1024 * 1024) * PART_MIN_LEN_MB)

enum {
  EXIT_OK,
  EXIT_FAIL,
  EXIT_GRAPH_COUNTED_OK,
  EXIT_CANT_ALLOC_MEM,
  EXIT_CANT_FORK,
  EXIT_NULL_TEXT,
  EXIT_NULL_GRAPHS,
  EXIT_NULL_KEY
};

int count_graph_in_text(unsigned long *count, const char *text,
                        const size_t text_len, const char *key) {
  if (!key) {
    fprintf(stderr, "Error: parse_text null ptr key\n");

    return EXIT_NULL_KEY;
  }

  // Вычисляем количество разбиений текста, чтобы параллельно считать на разных
  // участках текста
  long parts_count = sysconf(_SC_NPROCESSORS_ONLN);
  size_t step = 0;
  while ((long)(text_len / PART_MIN_LEN_BYTES) < parts_count &&
         parts_count > 1) {
    parts_count--;
  }
  step = text_len / parts_count + 1;

  //  Общая область памяти для параллельных вычислений
  long *shared_memory = mmap(NULL, sizeof(long), PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (!shared_memory) {
    fprintf(stderr,
            "Error: parse_text(): Cant allocate memory for  shared memory\n");

    return EXIT_CANT_ALLOC_MEM;
  }

  // выделяем память для хранения pid дочерних
  pid_t *part_pids = (pid_t *)malloc(parts_count * sizeof(pid_t));
  if (!part_pids) {
    fprintf(stderr,
            "Error parse_using_graph(): cant allocate memory for pids array\n");
    munmap(shared_memory, sizeof(long));

    return EXIT_CANT_ALLOC_MEM;
  }

  // Считаем смещение и ставим указатель на начало буфера
  const char *ptr = text;

  for (long i = 0; i < parts_count; i++) {
    part_pids[i] = fork();

    if (part_pids[i] == -1) {
      fprintf(stderr,
              "Error parse_using_graph(): Cant fork to separate text and "
              "count parallelly. %li\n",
              i);
      munmap(shared_memory, sizeof(long));
      free(part_pids);

      return EXIT_CANT_FORK;
    }

    if (!part_pids[i]) {
      //  Считаем количество данного диграфа в данном фрагменте длиной step
      *shared_memory += counter_simple(ptr, step, key);
      exit(EXIT_OK);
    }
    ptr += step;
  }

  for (long i = 0; i < parts_count; i++) {
    while (waitpid(part_pids[i], NULL, 0) > 0) {
    }
  }

  //  Переносим данные из общей области памяти и очищаем её
  *count += *shared_memory;

  free(part_pids);
  if (munmap(shared_memory, sizeof(long))) {
    fprintf(stderr, "Error parse_text(): Cannot unmap mem\n");

    return EXIT_FAIL;
  }

  return EXIT_OK;
}

int parse_text(const char *text, const size_t text_len, Graph **graphs,
               const size_t graphs_count) {
  //  printf("Using parallel parser...\n");

  //  Проверяем входные данные
  if (!text) {
    fprintf(stderr, "Error: parse_text null ptr text\n");

    return EXIT_NULL_TEXT;
  }
  if (!graphs) {
    fprintf(stderr, "Error: parse_text null ptr graphs\n");

    return EXIT_NULL_GRAPHS;
  }

  // Для каждого графа вызываем функцию подсчёта
  for (size_t i = 0; i < graphs_count; i++) {
    count_graph_in_text(&graphs[i]->count, text, text_len, graphs[i]->key);
  }
  //  Все графы посчитаны

  return EXIT_OK;
}
