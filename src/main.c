#include "digraphs.h"
#include "parallel_identif.h"

#include "stdlib.h"
#include "stdio.h"


#include <getopt.h>
#include <unistd.h>
#define EXIT_INCOR_FILEPATH 5
#define DEFAULT_MODE 1
#define EXIT_FOR_HELP 6
#define SEQ_PAR 4
#define BUFF_SIZE_MB 100
#define FILE_INPUT_NAME "/data/ex.txt"

int main(int argc, char *argv[]) {
 int opt = 0;
  int long_index = 0;
  char *file_path; int mode = DEFAULT_MODE;
    static struct option longopt[] = { 
        {"filepath", required_argument, 0,  'f' },
        {"mode", required_argument, 0, 'm'},

        {"help", no_argument, 0, 'h'}
    };  
    
    while ((opt = getopt_long(argc, argv,"f:m:", longopt, &long_index )) != -1) {
        switch (opt) {

            case 'h' :
            {
              printf("Usage:\n--mode <1 or 0>\n\t1 parallel\n\t0 sequential\n\n--filepath <path to data file>\n\n");
              exit(EXIT_FOR_HELP);
            }
            case 'm' :
            {

                mode = atoi(optarg);
                printf("\n%d\n", mode);
              break;
            } 
            case 'f' :
            { 
                // printf("\n%s\n", optarg);
                file_path = realpath (optarg, NULL);
                if (file_path) 
            {
              printf("%s\n", file_path);
               }
              else
               { 
                 printf("Error: incorrect filepath: \"%s\"\n\n", optarg);
                 return EXIT_INCOR_FILEPATH;
                }
                break;
            }
            default: 
                exit(1);
        }   
    }   
printf("ok\n");
return 0;


  size_t buff_size = BUFF_SIZE_MB * 1024 * 1024 / sizeof(char);
  char *buffer = (char *)malloc(buff_size);
  if (!buffer) {
    printf("Error: cannot alloc buffer for %i Mb\n", BUFF_SIZE_MB);
    return EXIT_FAILURE;
  }

  FILE *file = fopen(FILE_INPUT_NAME, "r");
  if (!file) {
    printf("Error: cannot open file %s\n", FILE_INPUT_NAME);
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

  Digraph digraphs[] = {{":)", 0}, {":(", 0}};
  size_t digraphs_count = 2;

if(indentification_parallel_all(buffer, buff_size, digraphs, digraphs_count, SEQ_PAR))
  printf("--%lu\n", digraphs[0].count);
  else
  printf(stderr);

  free(buffer);
  return EXIT_SUCCESS;
}