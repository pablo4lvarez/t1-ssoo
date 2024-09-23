#pragma once

#include <sys/types.h>

typedef struct {
  int pid;
  char* name;
  char* state; // RUNNING, READY, WAITING, FINISHED
  // int t_start;
  int t_cpu_burst;
  int n_burst;
  int t_io;
  int t_deadline;
} Process;

Process* create_process(int pid, char* name, int t_cpu_burst, int n_burst, int t_io, int t_deadline);

void print_process(Process* p);
