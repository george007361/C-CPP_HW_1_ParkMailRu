// / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "graphs.h"
#include "parser.h"

enum {
  EXIT_OK,
  EXIT_ERR,
  EXIT_NO_NEC_PARAMS,
  EXIT_INCOR_FILEPATH,
  EXIT_FOR_HELP,
  EXIT_CANT_CR_GRAPH,
  EXIT_CANT_SET_GRAPH,
  EXIT_CANT_MALLOC_MEM
};

#define DATA_BUFF_SIZE_MB 200
#define GRAPH_PTR_BUFF_SIZE 2

int main(int argc, char *argv[]) {
  // Выделяем переменные для парсинка аргументов,
  int opt = 0;
  int long_index = 0;
  char *file_path = NULL;  // "./data/ex.txt";
  int fpath_added = 0;
  size_t graphs_count = 0;
  size_t graphs_count_max = GRAPH_PTR_BUFF_SIZE;
  Graph **graphs = (Graph **)malloc(graphs_count_max * sizeof(Graph *));

  static struct option longopt[] = {{"filepath", required_argument, 0, 'f'},
                                    {"help", no_argument, 0, 'h'},
                                    {"graph", required_argument, 0, 'd'}};
  // Парсим аргументы
  while ((opt = getopt_long(argc, argv, "f:m:", longopt, &long_index)) != -1) {
    switch (opt) {
      default:

      case 'h': {
        printf(
            "Usage:\n"
            "--filepath <path to data file>\n\tNECESSARY ARG\n\n"
            "--graph \"<Graph name>\"\n\n"
            "--help for help\n\n");

        return EXIT_FOR_HELP;
      }

      case 'f': {
        file_path = realpath(optarg, NULL);

        if (file_path) {
          fpath_added = 1;
          //  printf("Attached file: %s\n", file_path);
        } else {
          fprintf(stderr, "Error main(): incorrect filepath: \"%s\"\n\n",
                  optarg);
          free_graphs(&graphs, &graphs_count);

          return EXIT_INCOR_FILEPATH;
        }

        break;
      }

      case 'd': {
        Graph *new_graph = create_graph();
        if (!new_graph) return EXIT_CANT_CR_GRAPH;
        if (!set_graph(new_graph, optarg, 0)) return EXIT_CANT_SET_GRAPH;

        if (graphs_count == graphs_count_max) {
          Graph **new_graphs =
              (Graph **)malloc(sizeof(Graph *) * (graphs_count_max *= 2));

          if (!new_graphs) {
            fprintf(stderr,
                    "Error main(): Cant realloc mem for new graphs array. "
                    "New size %lu\n",
                    graphs_count_max);
            free_graphs(&graphs, &graphs_count);

            return EXIT_CANT_MALLOC_MEM;
          }

          for (size_t i = 0; i < graphs_count; i++) new_graphs[i] = graphs[i];
          free(graphs);
          graphs = new_graphs;
        }

        graphs[graphs_count] = new_graph;
        graphs_count++;

        break;
      }
    }
  }
  // Проверяем наличие нужных параметров
  if (!fpath_added || !graphs_count) {
    printf(
        "There are not necessary args added. Use --help key for help\nFile "
        "Path to data file: %d\nGraphs added count: %lu\n\n",
        fpath_added, graphs_count);
    free_graphs(&graphs, &graphs_count);
    free(file_path);

    return EXIT_NO_NEC_PARAMS;
  }

  FILE *file = fopen(file_path, "r");
  if (!file) {
    fprintf(stderr, "Error main(): cannot open file %s\n", file_path);
    free_graphs(&graphs, &graphs_count);
    free(file_path);

    return EXIT_FAILURE;
  }

  //  Выделяем буфер заданного размера
  size_t buffer_size = DATA_BUFF_SIZE_MB * 1024 * 1024 / sizeof(char);
  char *buffer = (char *)malloc(buffer_size + 1);
  if (!buffer) {
    fprintf(stderr, "Error main(): cannot alloc buffer for %i Mb\n",
            DATA_BUFF_SIZE_MB);
    free_graphs(&graphs, &graphs_count);
    free(file_path);

    return EXIT_FAILURE;
  }

  struct timespec start_time, end_time;
  // заполняем буфер из файла и выполняем подсчёт количества включений искомого
  // графа в тексте по частям
  size_t bytes_read;
  clock_gettime(CLOCK_REALTIME, &start_time);
  while ((bytes_read = fread(buffer, sizeof(char), buffer_size, file)) > 0) {
    if (ferror(file)) {
      fprintf(stderr, "Error main(): cant read file %s\n", file_path);
      free_graphs(graphs, graphs_count);
      free(file_path);
      free(buffer);

      return EXIT_FAILURE;
    }
    parse_text(buffer, bytes_read, graphs, graphs_count);
  }
  clock_gettime(CLOCK_REALTIME, &end_time);

  if (fclose(file)) {
    fprintf(stderr, "Error: can't close file %s\n", file_path);
    free_graphs(&graphs, &graphs_count);
    free(file_path);
    free(buffer);

    return EXIT_FAILURE;
  }
  // Выводим резуьтат работы
  printf("\n\nIn file %s were found:\n", file_path);
  for (size_t i = 0; i < graphs_count; i++) {
    printf("\tGraph \"%s\" : %lu times\n", graphs[i]->key, graphs[i]->count);
  }
  double time_passed = (1000000 * (end_time.tv_sec - start_time.tv_sec) +
                        (end_time.tv_nsec - start_time.tv_nsec) / 1000);
  printf("\nTime passed: %.2lf ms\n\n", time_passed);
  //  Интерпретируем результат
  long d_count = 0;
  if (!interprier(&d_count, graphs, graphs_count, ":)", ":(")) {
    if (d_count > 0)
      printf(
          "Chat is positive. Count of :) bigger than count of :( more by %lu\n",
          d_count);
    else if (d_count < 0)

      printf(
          "Chat is negative. Count of :( bigger than count of :) more by %lu\n",
          -1 * d_count);
    else
      printf(
          "Cannot understand. Count of :) and :( graphs matches or not found "
          "in text\n");
  } else {
    fprintf(stderr, "Error interprier() in main(): cant interpritate result\n");
  }
  // Освобождаем память
  free_graphs(&graphs, &graphs_count);
  free(buffer);
  free(file_path);
  // Выход из программы, когда вся работа сделана. Должна вызываться один раз
  return EXIT_SUCCESS;
}
