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
  EXIT_CANT_ALLOC_MEM,
  EXIT_CANT_FORK,
  EXIT_NULL_TEXT,
  EXIT_NULL_GRAPHS,
  EXIT_NULL_KEY
};

int parse_using_graph(unsigned long *count, const char *text,
                      const size_t text_len, const char *key) {
  if (!key) {
    fprintf(stderr, "Error: parse_text null ptr key\n");
    return EXIT_NULL_KEY;
  }

  long system_has_processors = sysconf(_SC_NPROCESSORS_ONLN);

  pid_t *part_pids = (pid_t *)malloc(system_has_processors * sizeof(pid_t));
  if (!part_pids) {
    fprintf(stderr,
            "Error parse_using_graph(): cant allocate memory for pids array\n");

    return EXIT_CANT_ALLOC_MEM;
  }

  char *ptr = text;
  size_t step = text_len / system_has_processors + 1;

  for (long i = 0; i < system_has_processors; i++) {
    part_pids[i] = fork();
    if (part_pids[i] == -1) {
      fprintf(stderr,
              "Error parse_using_graph(): Cant fork to separate text and "
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

  for (long i = 0; i < system_has_processors; i++) {
    while (waitpid(part_pids[i], NULL, 0) > 0)
      ;
  }

  free(part_pids);

  return EXIT_OK;
}

int parse_text(const char *text, const size_t text_len, Graph **graphs,
               const size_t graphs_count) {
  // printf("Using parallel parser...\n");
  // Проверяем входные данные
  if (!text) {
    fprintf(stderr, "Error: parse_text null ptr text\n");

    return EXIT_NULL_TEXT;
  }
  if (!graphs) {
    fprintf(stderr, "Error: parse_text null ptr graphs\n");

    return EXIT_NULL_GRAPHS;
  }
  //Выделяем массив для хранения pid дочерних
  pid_t *graph_pids = (pid_t *)malloc(graphs_count * sizeof(pid_t));
  if (!graph_pids) {
    fprintf(stderr,
            "Error parse_text(): cant allocate memory for graphs pids array\n");

    return EXIT_CANT_ALLOC_MEM;
  }
  // Общая область памяти для параллельных вычислений
  unsigned long *count_arr_shared =
      mmap(NULL, graphs_count * sizeof(unsigned long), PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (!count_arr_shared) {
    fprintf(stderr,
            "Error: parse_text(): Cant allocate memory for  shared memory\n");
    free(graph_pids);

    return EXIT_CANT_ALLOC_MEM;
  }
  //Вычисляем количесто каждого графа(он же искомая подстрока) в тексте
  for (size_t i = 0; i < graphs_count; i++) {
    fflush(stdout);
    graph_pids[i] = fork();  // отделяем подсчёт i-ой подстроки в отдельный
                             // процесс, сохраняем его pid в массив
    if (graph_pids[i] == -1) {
      fprintf(stderr,
              "Error parse_text(): Cant fork to separate graphs parsing. %lu\n",
              i);
      free(graph_pids);
      if (munmap(count_arr_shared, graphs_count * sizeof(unsigned long))) {
        fprintf(stderr, "Error parse_text(): Cannot unmap mem\n");

        return EXIT_FAIL;
      }

      return EXIT_CANT_FORK;
    }

    if (!graph_pids[i]) {
      // Если это отделенный процесс, то Считаем количество i-ого графа в
      // тексте в отдельном процессе с помощью функции parse_using_graph, пока
      // закомментил, тк там аналогичная проблема

      parse_using_graph(&count_arr_shared[i], text, text_len, graphs[i]->key);

      exit(EXIT_GRAPH_COUNTED_OK);
    } else {
      // если это родитель, то ничего не делаем и идём на новую итерацию
    }
  }
  // Все подстроки посчитаны

  // Ждём дочерные процессы, которые считают количество искомой подстроки в
  // тексте по их зid
  for (size_t i = 0; i < graphs_count; i++) {
    while (waitpid(graph_pids[i], NULL, 0) > 0)
      ;
  }
  // Переносим данные из общей области на свои места
  for (size_t i = 0; i < graphs_count; i++)
    graphs[i]->count += count_arr_shared[i];

  if (munmap(count_arr_shared, graphs_count * sizeof(unsigned long))) {
    fprintf(stderr, "Error parse_text(): Cannot unmap mem\n");

    return EXIT_FAIL;
  }

  free(graph_pids);
  // Выходим, когда всё посчитали и можно давать ответ в main. Должна вызваться
  // один раз.
  return EXIT_OK;
}