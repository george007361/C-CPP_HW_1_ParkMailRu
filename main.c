#include <stdio.h>
#include <stdlib.h>

#include "computers.h"
#include "menu.h"

enum { ADD, OUT, EXIT };

int main() {
  List *computers;
  if (!init_list(&computers)) {
    printf("Error initialising list of computers\n");
    return EXIT_FAILURE;
  }

  Menu menu = init_menu(3, "ADD", "OUT", "EXIT");

  for (int key; (key = run_menu(&menu)) != EXIT;) {
    system("clear");
    switch (key) {
      case ADD: {
        if (!add_computer(computers)) printf("Error while adding car\n");

        break;
      }
      case OUT: {
        if (!group_computers(&computers)) printf("Error while grouping cars\n");
        if (!print_computers(computers))
          printf("Error while printg cars to console\n");

        break;
      }
      default: {
        break;
      }
    }
  }

  free_menu(&menu);
  if (!deinit_list(&computers)) printf("Error while freeing list of cars\n");

  return EXIT_SUCCESS;
}