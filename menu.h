#ifndef MENU_H
#define MENU_H

#define DEBUG_MENU 0

typedef struct Menu {
  char **items;
  int count;
} Menu;

Menu init_menu(int count, ...);
void free_menu(Menu *menu);
int run_menu(const Menu *m);

#endif