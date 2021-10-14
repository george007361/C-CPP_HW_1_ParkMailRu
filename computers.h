#ifndef PCINF_H
#define PCINF_H

#define DEBUG 0
#define NAME_MAX_LEN 50
#define SECURE_READ "%49s"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PC {
  char name[NAME_MAX_LEN];
  int cores;
  float freqMGhz;
  int ramMB;
} PC;

typedef struct Node Node;
struct Node {
  PC val;
  Node *next;
  Node *prev;
};

typedef struct List {
  Node *head;
  Node *tail;
  int count;
} List;

List *initList();
int deinitList(List **list);
int popTail(PC *data, List *list);
int pushHead(List *list, const PC data);
int pushTail(List *list, const PC data);
int getElem(Node **data, List *list, const int index);
int groupComputers(List **list);
int addComputer(List *computers);
int printComputers(const List *const list);
int delElem(PC *data, List *list, const int index);

#endif