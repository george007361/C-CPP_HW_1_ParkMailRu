#ifndef PCINF_H
#define PCINF_H

#define NAME_MAX_LEN 50

typedef struct PC {
  char name[NAME_MAX_LEN];
  int cores;
  float freqMGhz;
  int ramMB;
} PC;

int groupComputers();
int addComputer();
int printComputers();

#endif