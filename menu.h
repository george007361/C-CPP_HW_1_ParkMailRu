#ifndef MENU_H
#define MENU_H
#define DEBUG_MENU 1

#include <stdio.h>

typedef struct Menu {
  char **items;
  int count;
} Menu;

Menu InitMenu(int count, ...);
void FreeMenu(Menu *menu);
int RunMenu(const Menu *m);

#endif