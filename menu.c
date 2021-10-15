#include "menu.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"

Menu init_menu(int count, ...) {
  Menu menu;
  menu.items = malloc(count * sizeof(char *));
  if (!menu.items) {
#ifdef DEBUG
    printf("Menu init: cant alloc mem\n");
#endif  // DEBUG
    exit(1);
  }
  menu.count = count;

  va_list factor;           // указатель va_list
  va_start(factor, count);  // устанавливаем указатель

  for (int i = 0; i < count; i++) {
    menu.items[i] = va_arg(
        factor, char *);  // получаем значение текущего параметра типа int
  }
  va_end(factor);  // завершаем обработку параметров

  return menu;
}

void free_menu(Menu *menu) {
  free(menu->items);
  menu->items = NULL;
  menu->count = 0;
}

int run_menu(const Menu *menu) {
#if !DEBUG_MENU
#endif  // !DEBUG
  printf("\n\n");
  for (int i = 0; i < menu->count; i++) {
    printf("%i) %s\n", i + 1, menu->items[i]);
  }

  printf("> ");
  int pos;
  if (scanf("%i", &pos) != 1) {
    while (getchar() != '\n')
      ;
    pos = -1;
  }
  return pos - 1;
}