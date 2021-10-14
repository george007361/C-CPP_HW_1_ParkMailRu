#ifndef MENU_H
#define MENU_H
#define DEBUG_MENU 0

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "string.h"


typedef struct Menu {
  char **items;
  int count;
} Menu;

Menu initMenu(int count, ...);
void freeMenu(Menu *menu);
int runMenu(const Menu *m);

#endif