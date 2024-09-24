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
  int waiting_time; // Suma de todo los intervalos de tiempo en los que el proceso estuvo en estado WAITING o READY
  int current_state_time; // Tiempo en el que el proceso lleva en el estado actual
  int turnaround_time; // t finish - t start
  int sum_deadline;
  int priority;

} Process;

Process* create_process(char* name, int pid, int t_start, int t_cpu_burst, int n_burst, int t_io, int t_deadline);

void print_process(Process* p);






void write_csv(Process* processes, int len, char* file_name);