#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define DEFAULT_MODE 1
#define DATA_BUFF_SIZE_MB 100
#define GRAPH_PTR_BUFF_SIZE 2

int main(int argc, char *argv[]) {
  int opt = 0;
  int long_index = 0;
  char *file_path = 0;  //"./data/ex.txt";
  int fpath_added = 0;
  int mode = DEFAULT_MODE;
  size_t graphs_count = 0;
  size_t graphs_count_max = GRAPH_PTR_BUFF_SIZE;
  Graph **graphs = (Graph **)malloc(graphs_count_max * sizeof(Graph *));

  static struct option longopt[] = {{"filepath", required_argument, 0, 'f'},
                                    {"mode", required_argument, 0, 'm'},
                                    {"help", no_argument, 0, 'h'},
                                    {"graph", required_argument, 0, 'd'}};

  while ((opt = getopt_long(argc, argv, "f:m:", longopt, &long_index)) != -1) {
    switch (opt) {
      default:

      case 'h': {
        printf(
            "Usage:\n"
            "--mode <1 or 0>\n\t1 parallel\n\t0 sequential\n\tDefault: 1\n\n"
            "--filepath <path to data file>\n\tNECESSARY ARG\n\n"
            "--graph \"<Graph name>\"\n\n"
            "--help for help\n\n");

        return EXIT_FOR_HELP;
      }

      case 'm': {
        mode = atoi(optarg);
        printf("\n%d\n", mode);

        break;
      }

      case 'f': {
        file_path = realpath(optarg, NULL);

        if (file_path) {
          fpath_added = 1;
          printf("%s\n", file_path);
        } else {
          fprintf(stderr, "Error main(): incorrect filepath: \"%s\"\n\n",
                  optarg);

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
            free(graphs);
            fprintf(stderr,
                    "Error main(): Cant realloc mem for new graphs array. "
                    "New size %lu\n",
                    graphs_count_max);

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

  if (!fpath_added) {
    printf(
        "There are not necessary args added. Use --help key for help\nFile "
        "path: %d\n Graphs added count: %lu\n\n",
        fpath_added, graphs_count);

    return EXIT_NO_NEC_PARAMS;
  }

  size_t buff_size = DATA_BUFF_SIZE_MB * 1024 * 1024 / sizeof(char);
  char *buffer = (char *)malloc(buff_size);
  if (!buffer) {
    fprintf(stderr, "Error main(): cannot alloc buffer for %i Mb\n",
            DATA_BUFF_SIZE_MB);

    return EXIT_FAILURE;
  }

  FILE *file = fopen(file_path, "r");

  if (!file) {
    fprintf(stderr, "Error main(): cannot open file %s\n", file_path);

    return EXIT_FAILURE;
  }

  size_t bytes_read;
  while (bytes_read = fread(buffer, sizeof(char), buff_size, file)) {
    if (ferror(file)) {
      fprintf(stderr, "Error main(): cant read file %s\n", file_path);

      return EXIT_FAILURE;
    }
    parse_text(buffer, bytes_read, graphs, graphs_count);
  }

  if (fclose(file)) {
    fprintf(stderr, "Error: can't close file %s\n", file_path);

    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < graphs_count; i++) {
    printf("%s : %lu\n", graphs[i]->key, graphs[i]->count);
  }

  for (size_t i = 0; i < graphs_count; i++) {
    free_graph(graphs[i]);
  }
  free(graphs);
  free(buffer);
  free(file_path);

  return EXIT_SUCCESS;
}
