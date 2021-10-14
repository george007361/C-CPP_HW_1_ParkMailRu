#include <stdio.h>
#include <stdlib.h>

#include "computers.h"
#include "menu.h"

enum { ADD, OUT, EXIT };

int main() {
  List *computers = initList();
  Menu menu = InitMenu(3, "ADD", "OUT", "EXIT");

  for (int key; (key = RunMenu(&menu)) != EXIT;) {
    system("clear");
    switch (key) {
      case ADD: {
        if (!addComputer(computers)) printf("Error while adding car\n");

        break;
      }
      case OUT: {
        if (!printComputers(computers))
          printf("Error while printg cars to console\n");

        break;
      }
      default: {
        break;
      }
    }
  }

  FreeMenu(&menu);
  if (!deinitList(&computers)) printf("Error while freeing list of cars\n");

  return 0;
}