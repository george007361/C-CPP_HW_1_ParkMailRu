#include <getopt.h>
#include <string.h>
#include <unistd.h>

#include "digraphs.h"
#include "parallel_identif.h"
#include "stdio.h"
#include "stdlib.h"

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
  char *file_path;
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
      case 'h': {
        printf(
            "Usage:\n"
            "--mode <1 or 0>\n\t1 parallel\n\t0 sequential\n\tDefault: 1\n\n"
            "--filepath <path to data file>\n\tNECESSARY ARG\n\n"
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
          printf("Error: incorrect filepath: \"%s\"\n\n", optarg);
          return EXIT_INCOR_FILEPATH;
        }
        break;
      }
      case 'd': {
        Digraph *new_digraph = create_digraph();
        if (!new_digraph) {
          printf(stderr);
          return EXIT_CANT_CR_DIGRAPH;
        }
        if (!set_digraph(new_digraph, optarg, 0)) {
          printf(stderr);
          return EXIT_CANT_SET_DIGRAPH;
        }
        if (digraphs_count == digraphs_count_max) {
          Digraph **new_digraphs =
              (Digraph **)malloc((digraphs_count_max *= 2));
          if (!new_digraphs) {
            free(digraphs);
            printf("Cant realloc mem for new digraphs arra. new size %d\n",
                   digraphs_count_max);
            return EXIT_CANT_MALLOC_MEM;
          }
          memcpy(new_digraphs, digraphs, digraphs_count);
          free(digraphs);
          digraphs = new_digraphs;
        }
        digraphs[digraphs_count] = new_digraph;
        digraphs_count++;
        break;
      }
      default:
        exit(666);
    }
  }

  if (!fpath_added) {
    printf(
        "There are not necessary args added. Use --help key for help\nFile "
        "path: %d\n digraphs added count: %d\n\n",
        fpath_added, digraphs_count);
    return EXIT_NO_NEC_PARAMS;
  }

  // free_digraph(sad);
  // for (size_t i = 0; i < digraphs_count; i++)
  // {
  //   free_digraph(digraphs[i]);
  // }
  // free(digraphs);
  // return EXIT_OK;









  size_t buff_size = BUFF_SIZE_MB * 1024 * 1024 / sizeof(char);
  char *buffer = (char *)malloc(buff_size);
  if (!buffer) {
    printf("Error: cannot alloc buffer for %i Mb\n", BUFF_SIZE_MB);
    return EXIT_FAILURE;
  }

  FILE *file = fopen(file_path, "r");
  if (!file) {
    printf("Error: cannot open file %s\n", file_path);
    return EXIT_FAILURE;
  }

  fseek(file, 0, SEEK_END);
  size_t f_real_len = ftell(file);
  rewind(file);
  buff_size = f_real_len > buff_size ? buff_size : f_real_len;

  if (!fread(buffer, buff_size, sizeof(char), file)) {
    printf("Error: cant read file\n");
    return EXIT_FAILURE;
  }

  if (fclose(file)) {
    printf("Error: can't close file\n");
    return EXIT_FAILURE;
  }

  puts(buffer);

  // size_t digraphs_count = 2;
  // Digraph **digraphs = (Digraph **)malloc(sizeof(Digraph *) *
  // digraphs_count); if (!digraphs) {
  //   printf("Can't alloc mem for digraph is main()\n");
  //   return EXIT_FAILURE;
  // }

  // Digraph *happy, *sad = create_digraph();
  // if (!happy || !sad) {
  //   printf(stderr);
  //   return EXIT_FAILURE;
  // }

  // if (!set_digraph(happy, ":)", 0) || !set_digraph(sad, ":(", 0)) {
  //   printf(stderr);
  //   return EXIT_FAILURE;
  // }

  // digraphs[0] = happy;
  // digraphs[1] = sad;
  // for (size_t i = 0; i < digraphs_count; i++) {
  //   printf("%s ", digraphs[i]->key);
  // }
  // printf("\n\n");

  if (indentification_parallel_all(buffer, buff_size, *digraphs,
                                   digraphs_count))
    printf("--%lu\n", digraphs[0]->count);
  else
    printf(stderr);

  // free_digraph(happy);
  // free_digraph(sad);
  for (size_t i = 0; i < digraphs_count; i++)
  {
    free_digraph(digraphs[i]);
  }
  free(digraphs);
  
  free(buffer);
  printf("main\n");
  return EXIT_SUCCESS;
}