#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Function to create a new process
Process* create_process(int pid, char* name, int t_cpu_burst, int n_burst, int t_io, int t_deadline) {
  Process* p = (Process*) malloc(sizeof(Process));
  p->pid = pid;
  p->name = name;
  p->state = "READY";
  p->t_cpu_burst = t_cpu_burst;
  p->n_burst = n_burst;
  p->t_io = t_io;
  p->t_deadline = t_deadline;
  return p;
}

// Function to print a process
void print_process(Process* p) {
  printf("Printing a process with the PID: %d\n", p->pid);
  printf("Process %s\n", p->name);
  printf("State: %s\n", p->state);
  printf("CPU Burst Time: %d\n", p->t_cpu_burst);
  printf("Number of Bursts: %d\n", p->n_burst);
  printf("I/O Time: %d\n", p->t_io);
  printf("Deadline: %d\n", p->t_deadline);
}
