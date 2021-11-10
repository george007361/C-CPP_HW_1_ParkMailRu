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

#define DATA_BUFF_SIZE_MB_DEFAULT 50
#define DATA_BUFF_SIZE_MB_MIN 50
#define GRAPH_PTR_BUFF_SIZE 2
#define MB_TO_BYTES (1024 * 1024)
#define SEC_TO_MS 1000000
#define MS_TO_US 1000

int main(int argc, char *argv[]) {
  // Выделяем переменные для парсинка аргументов,
  int opt = 0;
  int long_index = 0;
  char *file_path = NULL;  // "./data/ex.txt";
  int fpath_added = 0;
  size_t graphs_count = 0;
  size_t graphs_count_max = GRAPH_PTR_BUFF_SIZE;
  Graph **graphs = (Graph **)malloc(graphs_count_max * sizeof(Graph *));
  int buffer_size = DATA_BUFF_SIZE_MB_DEFAULT;
  int error_occured = 0;
  static struct option longopt[] = {{"filepath", required_argument, 0, 'f'},
                                    {"help", no_argument, 0, 'h'},
                                    {"graph", required_argument, 0, 'd'},
                                    {"bufsize", required_argument, 0, 'b'},
                                    {NULL, 0, 0, 0}};
  // Парсим аргументы
  while ((opt = getopt_long(argc, argv, "f: m:", longopt, &long_index)) != -1 &&
         !error_occured) {
    switch (opt) {
        // --help
      default:

      case 'h': {
        printf(
            "Usage:\n"
            "--filepath <path to data file>\n\tNECESSARY ARG\n\n"
            "--graph \"<Graph name>\"\n\n"
            "--bufsize <new buffer size> DEFAULT: %i\n\n"
            "--help for help\n\n",
            DATA_BUFF_SIZE_MB_DEFAULT);

        error_occured = EXIT_FOR_HELP;
        break;
      }

        // --buffsize
      case 'b': {
        if (atoi(optarg) >= DATA_BUFF_SIZE_MB_MIN) {
          buffer_size = atoi(optarg);
        } else {
          printf(
              "New buffer size shold be at least %i Mb. Using default buffer "
              "size %i\n",
              DATA_BUFF_SIZE_MB_MIN, DATA_BUFF_SIZE_MB_DEFAULT);
        }

        break;
      }

        // --filepath
      case 'f': {
        file_path = realpath(optarg, NULL);
        if (file_path) {
          fpath_added = 1;
        } else {
          fprintf(stderr, "Error main(): incorrect filepath: \"%s\"\n\n",
                  optarg);

          error_occured = EXIT_INCOR_FILEPATH;
          break;
        }

        break;
      }

        // --graph
      case 'd': {
        Graph *new_graph = create_graph();
        if (!new_graph) {
          fprintf(stderr, "Error main(): Cant allocate mem for new graph\n");
          error_occured = EXIT_CANT_CR_GRAPH;
          break;
        }
        if (!set_graph(new_graph, optarg, 0)) {
          error_occured = EXIT_CANT_SET_GRAPH;
          break;
        }

        if (graphs_count == graphs_count_max) {
          Graph **new_graphs =
              (Graph **)malloc(sizeof(Graph *) * (graphs_count_max *= 2));

          if (!new_graphs) {
            fprintf(stderr,
                    "Error main(): Cant realloc mem for new graphs array. "
                    "New size %zu\n",
                    graphs_count_max);

            error_occured = EXIT_CANT_MALLOC_MEM;
            break;
          }

          for (size_t i = 0; i < graphs_count; i++) {
            new_graphs[i] = graphs[i];
          }
          free(graphs);
          graphs = new_graphs;
        }

        graphs[graphs_count] = new_graph;
        graphs_count++;

        break;
      }
    }
  }

  //  Если произошла ошибка выходим
  if (error_occured) {
    free_graphs(&graphs, &graphs_count);
    free(file_path);

    return error_occured;
  }

  // Проверяем наличие нужных аргументов
  if (!fpath_added || !graphs_count) {
    printf(
        "There are not necessary args added. Use --help key for help\nFile "
        "Path to data file: %d\nGraphs added count: %zu\n\n",
        fpath_added, graphs_count);
    free_graphs(&graphs, &graphs_count);
    free(file_path);

    return EXIT_NO_NEC_PARAMS;
  }

  //  открываем файл
  FILE *file = fopen(file_path, "r");
  if (!file) {
    fprintf(stderr, "Error main(): cannot open file %s\n", file_path);
    free_graphs(&graphs, &graphs_count);
    free(file_path);

    return EXIT_FAILURE;
  }

  //  Выделяем буфер заданного размера
  size_t buffer_size_bytes = buffer_size * MB_TO_BYTES / sizeof(char);
  char *buffer = (char *)malloc(buffer_size_bytes + 1);
  if (!buffer) {
    fprintf(stderr, "Error main(): cannot alloc buffer for %i Mb\n",
            buffer_size);
    free_graphs(&graphs, &graphs_count);
    free(file_path);

    return EXIT_FAILURE;
  }

  // заполняем буфер из файла и выполняем подсчёт количества включений искомого
  // графа в тексте по частям, измеряем время
  struct timespec start_time;
  struct timespec end_time;
  size_t bytes_read = 0;

  clock_gettime(CLOCK_REALTIME, &start_time);

  while ((bytes_read = fread(buffer, sizeof(char), buffer_size_bytes, file)) >
         0) {
    if (ferror(file)) {
      fprintf(stderr, "Error main(): cant read file %s\n", file_path);
      free_graphs(&graphs, &graphs_count);
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
  time_t time_passed = (SEC_TO_MS * (end_time.tv_sec - start_time.tv_sec) +
                        (end_time.tv_nsec - start_time.tv_nsec) / MS_TO_US);
  printf("\nTime passed: %li ms\n\n", time_passed);

  //  Интерпретируем результат
  long d_count = 0;
  if (!interprier(&d_count, graphs, graphs_count, ":)", ":(")) {
    if (d_count > 0) {
      printf(
          "Chat is positive. Count of :) bigger than count of :( more by %li\n",
          d_count);
    } else if (d_count < 0) {
      printf(
          "Chat is negative. Count of :( bigger than count of :) more by %li\n",
          -1 * d_count);
    } else {
      printf(
          "Cannot understand. Count of :) and :( graphs matches or not found "
          "in text\n\n");
    }
  } else {
    fprintf(stderr,
            "Error interprier() in main(): cant interpritate result\n\n");
  }

  // Освобождаем память
  free_graphs(&graphs, &graphs_count);
  free(buffer);
  free(file_path);

  // Выход из программы, когда вся работа сделана. Должна вызываться один раз
  return EXIT_SUCCESS;
}
