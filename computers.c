#include "computers.h"

List *initList() {
  List *list = (List *)malloc(sizeof(List));

  list->head = list->tail = NULL;
  list->count = 0;

  return list;
}

int deinitList(List **list) {
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

int popTail(PC *data, List *list) {
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

int pushTail(List *list, const PC data) {
  if (!list) {
#if DEBUG
    printf("Error: nullprt list in pushTail\n");
#endif  // DEBUG
    return 0;
  }

  Node *tmp = (Node *)malloc(sizeof(Node));

  tmp->val = data;
  tmp->prev = tmp->next = NULL;
  if (list->head == NULL && list->tail == NULL)
    list->head = list->tail = tmp;
  else {
    tmp->prev = list->tail;
    list->tail->next = tmp;
    list->tail = tmp;
  }
  ++list->count;

  return 1;
}

void printComputer(PC computer) {
  printf("PC Name: %s\nCount of PC's cores: %i\nCPU freq: %f\nRAM size: %i\n\n",
         computer.name, computer.cores, computer.freqMGhz, computer.ramMB);
}

int printComputers(const List *const list) {
  if (!list || ((!list->head || !list->tail) && list->count > 0)) {
#if DEBUG
    printf("Error: printComputers nullptr or damaged list\n");
#endif  // DEBUG
    return 0;
  }

  Node *tmp = list->head;

  for (int i = 0; i < list->count; i++) {
    printf("%i) ", i + 1);
    printComputer(tmp->val);
    tmp = tmp->next;
  }

  printf("Type q to leave...\n");
  while (getchar() != 'q')
    ;

  return 1;
}

int addComputer(List *computers) {
  PC new_pc;

  printf("\nType PC Name: ");
  scanf(SECURE_READ, &new_pc.name);

  printf("Type count of PC's cores: ");
  //  getchar();
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

  return pushTail(computers, new_pc);
}