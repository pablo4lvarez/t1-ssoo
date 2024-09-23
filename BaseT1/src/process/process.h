#pragma once

#include <sys/types.h>

typedef struct {
  // Atributos iniciales
  int pid;
  char* name;
  char* state; // RUNNING, READY, WAITING, FINISHED
  int t_cpu_burst;
  int n_burst;
  int t_io;
  int t_deadline;

  // Atributos extra
  int t_start; // tick en el que se inicia
  int t_finish; // tick en el que se termina
  int n_interrupts;
  int t_lcpu; // Tiempo en CPU, parte desde el quantum y se decrementa
  int waiting_time;
  int turnaround_time; // t finish - t start
  int sum_deadline;

} Process;

Process* create_process(int pid, char* name, int t_cpu_burst, int n_burst, int t_io, int t_deadline, int quantum);

void print_process(Process* p);
