#include <stdio.h>
#include <stdlib.h>

#include "computers.h"

int main() {
  List *computers = initList();

  if (!addComputer(computers)) printf("Error while adding car\n");
  if (!printComputers(computers))
    printf("Error while printg cars to console\n");

  if (!deinitList(&computers)) printf("Error while freeing list of cars\n");

  return 0;
}