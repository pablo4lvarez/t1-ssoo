#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Function to create a new process
Process* create_process(int pid, char* name, int t_cpu_burst, int n_burst, int t_io, int t_deadline, int quantum) {
  Process* p = (Process*) malloc(sizeof(Process));
  p->pid = pid;
  p->name = name;
  p->state = "READY";
  p->t_cpu_burst = t_cpu_burst;
  p->n_burst = n_burst;
  p->t_io = t_io;
  p->t_deadline = t_deadline;

  p->t_lcpu = quantum;
  p->t_start = 0;
  p->t_finish = 0;
  p->n_interrupts = 0;
  p->waiting_time = 0;
  p->turnaround_time = 0;
  p->sum_deadline = 0;

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
  printf('The other attributes are: \n');
  printf("Start Time: %d\n", p->t_start);
  printf("Finish Time: %d\n", p->t_finish);
  printf("Number of Interrupts: %d\n", p->n_interrupts);
  printf("Time left in CPU: %d\n", p->t_lcpu);
  printf("Waiting Time: %d\n", p->waiting_time);
  printf("Turnaround Time: %d\n", p->turnaround_time);
  printf("Sum of Deadlines: %d\n", p->sum_deadline);
}
