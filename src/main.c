#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "digraphs.h"
#include "parallel_parser.h"

enum {
  EXIT_OK,
  EXIT_ERR,
  EXIT_NO_NEC_PARAMS,
  EXIT_INCOR_FILEPATH,
  EXIT_FOR_HELP,
  EXIT_CANT_CR_DIGRAPH,
  EXIT_CANT_SET_DIGRAPH,
  EXIT_CANT_MALLOC_MEM
};

#define DEFAULT_MODE 1
#define DATA_BUFF_SIZE_MB 100
#define DIGRAPH_PTR_BUFF_SIZE 2
#define BUFF_SIZE_MB 100

int main(int argc, char *argv[]) {
  int opt = 0;
  int long_index = 0;
  char *file_path = 0;  //"./data/ex.txt";
  int fpath_added = 0;
  int mode = DEFAULT_MODE;
  size_t digraphs_count = 0;
  size_t digraphs_count_max = DIGRAPH_PTR_BUFF_SIZE;
  Digraph **digraphs =
      (Digraph **)malloc(digraphs_count_max * sizeof(Digraph *));

  static struct option longopt[] = {{"filepath", required_argument, 0, 'f'},
                                    {"mode", required_argument, 0, 'm'},
                                    {"help", no_argument, 0, 'h'},
                                    {"digraph", required_argument, 0, 'd'}};

  while ((opt = getopt_long(argc, argv, "f:m:", longopt, &long_index)) != -1) {
    switch (opt) {
      default:

      case 'h': {
        printf(
            "Usage:\n"
            "--mode <1 or 0>\n\t1 parallel\n\t0 sequential\n\tDefault: 1\n\n"
            "--filepath <path to data file>\n\tNECESSARY ARG\n\n"
            "--digraph \"<Digraph name>\"\n\n"
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
        Digraph *new_digraph = create_digraph();
        if (!new_digraph) return EXIT_CANT_CR_DIGRAPH;
        if (!set_digraph(new_digraph, optarg, 0)) return EXIT_CANT_SET_DIGRAPH;

        if (digraphs_count == digraphs_count_max) {
          Digraph **new_digraphs =
              (Digraph **)malloc(sizeof(Digraph *) * (digraphs_count_max *= 2));

          if (!new_digraphs) {
            free(digraphs);
            fprintf(stderr,
                    "Error main(): Cant realloc mem for new digraphs array. "
                    "New size %lu\n",
                    digraphs_count_max);

            return EXIT_CANT_MALLOC_MEM;
          }

          for (size_t i = 0; i < digraphs_count; i++)
            new_digraphs[i] = digraphs[i];
          free(digraphs);
          digraphs = new_digraphs;
        }

        digraphs[digraphs_count] = new_digraph;
        digraphs_count++;

        break;
      }
    }
  }

  if (!fpath_added) {
    printf(
        "There are not necessary args added. Use --help key for help\nFile "
        "path: %d\n Digraphs added count: %lu\n\n",
        fpath_added, digraphs_count);

    return EXIT_NO_NEC_PARAMS;
  }

  size_t buff_size = BUFF_SIZE_MB * 1024 * 1024 / sizeof(char);
  char *buffer = (char *)malloc(buff_size);
  if (!buffer) {
    fprintf(stderr, "Error main(): cannot alloc buffer for %i Mb\n",
            BUFF_SIZE_MB);

    return EXIT_FAILURE;
  }

  FILE *file = fopen(file_path, "r");

  if (!file) {
    fprintf(stderr, "Error main(): cannot open file %s\n", file_path);

    return EXIT_FAILURE;
  }

  fseek(file, 0, SEEK_END);
  size_t f_real_len = ftell(file);
  rewind(file);
  buff_size = f_real_len > buff_size ? buff_size : f_real_len;

  if (!fread(buffer, buff_size, sizeof(char), file)) {
    fprintf(stderr, "Error main(): cant read file %s\n", file_path);

    return EXIT_FAILURE;
  }

  if (fclose(file)) {
    fprintf(stderr, "Error: can't close file %s\n", file_path);

    return EXIT_FAILURE;
  }

  // puts(buffer);

  parse_text(buffer, buff_size - 1, digraphs, digraphs_count);

  for (size_t i = 0; i < digraphs_count; i++) {
    printf("%s : %lu\n", digraphs[i]->key, digraphs[i]->count);
  }

  for (size_t i = 0; i < digraphs_count; i++) {
    free_digraph(digraphs[i]);
  }
  free(digraphs);
  free(buffer);
  free(file_path);

  return EXIT_SUCCESS;
}
