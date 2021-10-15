#include "computers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_list(List **list) {
  *list = (List *)malloc(sizeof(List));
  if (!(*list)) {
#if DEBUG
    printf("init_list(): malloc error");
#endif  // DEBUG
    return 0;
  }
  (*list)->head = (*list)->tail = NULL;
  (*list)->count = 0;

  return 1;
}

int deinit_list(List **list) {
  if (!(*list)) {
#if DEBUG
    printf("Empty list ptr in deinitList\n");
#endif  // DEBUG
    return 0;
  }

  Node *tmp = (*list)->head;
  Node *next = NULL;

  while (tmp) {
    next = tmp->next;
    free(tmp);
    tmp = next;
  }

  free(*list);
  (*list) = NULL;

  return 1;
}

int pop_tail(PC *data, List *list) {
  if (!list || !list->tail) {
#ifdef DEBUG

#endif  // DEBUG
    return 0;
  }

  Node *tmp = list->tail;

  list->tail = list->tail->prev;
  if (list->tail) {
    list->tail->next = NULL;
  }
  if (tmp == list->head) {
    list->head = NULL;
  }
  *data = tmp->val;
  list->count--;

  free(tmp);

  return 1;
}

int push_tail(List *list, const PC data) {
  if (!list) {
#if DEBUG
    printf("Error: nullprt list in pushTail\n");
#endif  // DEBUG
    return 0;
  }

  Node *tmp = (Node *)malloc(sizeof(Node));
  if (!tmp) {
#if DEBUG
    printf("push_tail: malloc error\n");
#endif  // DEBUG
    return 0;
  }
  tmp->val = data;
  tmp->prev = tmp->next = NULL;
  if (list->head == NULL && list->tail == NULL) {
    list->head = list->tail = tmp;
  } else {
    tmp->prev = list->tail;
    list->tail->next = tmp;
    list->tail = tmp;
  }
  ++list->count;

  return 1;
}

int get_elem(Node **data, List *list, const int index) {
  if (!list || !list->head || !list->tail || index < 0 ||
      index >= list->count) {
#if DEBUG
    printf("Error access data. Check prts and index in getElem\n");
#endif  // DEBUG
    return 0;
  }

  *data = list->head;
  for (size_t i = 0; i < index; i++) {
    *data = (*data)->next;
  }

  return 1;
}

int get_and_remove(PC *data, List *list, const int index) {
  Node *elem;
  if (!get_elem(&elem, list, index)) {
#if DEBUG
    printf("Error while deleting element: can't get necessary elem\n");
#endif  // DEBUG
    return 0;
  }

  if (elem->prev) {
    elem->prev->next = elem->next;
  } else {
    list->head = elem->next;
  }
  if (elem->next) {
    elem->next->prev = elem->prev;
  } else {
    list->tail = elem->prev;
  }
  *data = elem->val;
  list->count--;

  free(elem);

  return 1;
}

int group_computers(List **list) {
  if (!(*list)) {
#if DEBUG
    printf("Error: nullptr in groupComputers\n");
#endif  // DEBUG
    return 0;
  }

  List *grouped;
  if (!init_list(&grouped)) {
#if DEBUG
    printf("Error\n");
#endif  // DEBUG
    return 0;
  }

  while ((*list)->count) {
    PC tmp;

    get_and_remove(&tmp, (*list), 0);
    push_tail(grouped, tmp);

    for (int i = 0; i < (*list)->count;) {
      Node *data;

      get_elem(&data, (*list), i);

      if (!strcmp(data->val.name, grouped->tail->val.name)) {
        PC tmp2;

        get_and_remove(&tmp2, (*list), i);
        push_tail(grouped, tmp2);
      } else {
        i++;
      }
    }
  }

  deinit_list(list);
  (*list) = grouped;

  return 1;
}

void print_computer(PC computer) {
  printf("PC Name: %s\nCount of PC's cores: %i\nCPU freq: %f\nRAM size: %i\n\n",
         computer.name, computer.cores, computer.freqMGhz, computer.ramMB);
}

int print_computers(const List *const list) {
  if (!list || ((!list->head || !list->tail) && list->count > 0)) {
#if DEBUG
    printf("Error: printComputers nullptr or damaged list\n");
#endif  // DEBUG
    return 0;
  }

  Node *tmp = list->head;

  for (int i = 0; i < list->count; i++) {
    printf("%i) ", i + 1);
    print_computer(tmp->val);
    tmp = tmp->next;
  }

  printf("Type q to leave...\n");
  while (getchar() != 'q')
    ;

  return 1;
}

int add_computer(List *computers) {
  PC new_pc;

  printf("\nType PC Name: ");
  scanf(SECURE_READ, &new_pc.name);

  printf("Type count of PC's cores: ");
  if (scanf(" %d", &new_pc.cores) != 1) {
    printf("Error of input cores. Check format\n");
    while (getchar() != '\n')
      ;
    return 0;
  }

  printf("Type CPU freq: ");
  if (scanf(" %f", &new_pc.freqMGhz) != 1) {
    printf("Error of input CPU freq. Check format\n");
    while (getchar() != '\n')
      ;
    return 0;
  }

  printf("Type RAM size: ");
  if (scanf(" %i", &new_pc.ramMB) != 1) {
    printf("Error of input RAM size. Check format\n");
    while (getchar() != '\n')
      ;
    return 0;
  }

  return push_tail(computers, new_pc);
}