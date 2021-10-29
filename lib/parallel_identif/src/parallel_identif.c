#include "parallel_identif.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int indentification_parallel(const char *array, const size_t array_len, Digraph *digraph, const unsigned separates_count)
{
  pid_t *curr_task_pids = (pid_t *) malloc(separates_count * sizeof(pid_t));
  if(!curr_task_pids)
  {
    sprintf(stderr, "Can not allocate mem for pids in indentification_parallel()\n");
    return EXIT_FAILURE;
  }
      size_t *shared_memory = mmap(NULL, sizeof(size_t), PROT_READ | PROT_WRITE,
                                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
      if (!shared_memory) {
        printf("Failed to map\n");
        return EXIT_FAILURE;
      }
      *shared_memory = 0;
      // size_t step = len / processors + 1;

  for (size_t i = 0; i < separates_count; i++)
  {
    pid_t curr_task_pid = fork();
    curr_task_pids[i] = curr_task_pid;
    if(curr_task_pid == -1)
    
    {
      sprintf(stderr, "Cant forn in indentification_parallel()\n");
      return EXIT_FAILURE;
    }
    if(!curr_task_pid)
    {
      // sleep(1);
      printf("task %d\n", i);
      *shared_memory += i;
      exit(0);
    }
  }
  for (size_t i = 0; i < separates_count; i++)
  {
    printf("wait task %d %d\n", i, curr_task_pids[i]);
    while( waitpid(curr_task_pids[i], NULL, 0) > 0);
  }
  
  printf("--- %d ---\n", *shared_memory);

if(munmap(shared_memory, sizeof(size_t)))
{
  sprintf(stderr, "Cannot unmap mem\n");
  return EXIT_FAILURE;
}

  free(curr_task_pids);
  return EXIT_SUCCESS;
}

int indentification_parallel_all(const char *array,const size_t array_len, Digraph *digraphs_arr, const size_t digraphs_arr_len, const unsigned separates_count)
{
  pid_t *digr_pids = (pid_t *) malloc(digraphs_arr_len * sizeof(pid_t));
  if(!digr_pids)
  {
    sprintf(stderr, "Can not allocate mem for pids in indentification_parallel_all()\n");
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < digraphs_arr_len; i++)
  {
    pid_t digr_curr_pid = fork();
    digr_pids[i] = digr_curr_pid;
    if(digr_curr_pid == -1)
    
    {
      sprintf(stderr, "Cant forn in indentification_parallel_all()\n");
      return EXIT_FAILURE;
    }
    if(!digr_curr_pid)
    {
      printf("%d\n", i);
      // exit(0);
      exit(indentification_parallel(array, array_len, &digraphs_arr[i], separates_count));
    }
  }
  for (size_t i = 0; i < digraphs_arr_len; i++)
  {
    while(waitpid(digr_pids[i], NULL, 0) > 0);
  }
  
  free(digr_pids);
  return EXIT_SUCCESS;
}