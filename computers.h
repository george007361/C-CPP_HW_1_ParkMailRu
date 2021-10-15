#ifndef PCINF_H
#define PCINF_H

#define DEBUG 0
#define NAME_MAX_LEN 50
#define SECURE_READ "%49s"

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

int init_list(List **list);
int deinit_list(List **list);
int pop_tail(PC *data, List *list);
int push_head(List *list, const PC data);
int push_tail(List *list, const PC data);
int get_elem(Node **data, List *list, const int index);
int group_computers(List **list);
int add_computer(List *computers);
int print_computers(const List *const list);
int get_and_remove(PC *data, List *list, const int index);

#endif