#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Function to create a new process
Process* create_process(char* name, int pid, int t_start, int t_cpu_burst, int n_burst, int t_io, int t_deadline) {
  Process* p = (Process*) malloc(sizeof(Process));
  p->pid = pid;
  p->name = name;
  p->t_start = t_start;
  p->state = "READY";
  p->t_cpu_burst = t_cpu_burst;
  p->n_burst = n_burst;
  p->t_io = t_io;
  p->t_deadline = t_deadline;

  p->t_lcpu = 0;
  p->t_finish = 0;
  p->n_interrupts = 0;
  p->waiting_time = 0;
  p->turnaround_time = 0;
  p->sum_deadline = 0;
  p->priority = -999999999;
  p->current_state_time = 0;
  p->first_execution_time = -1;

  return p;
}

// Function to print a process
void print_process(Process* p) {
  printf("Printing a process with the PID: %d\n", p->pid);
  printf("Process %s\n", p->name);
  printf("Start Time: %d\n", p->t_start);
  printf("State: %s\n", p->state);
  printf("CPU Burst Time: %d\n", p->t_cpu_burst);
  printf("Number of Bursts: %d\n", p->n_burst);
  printf("I/O Time: %d\n", p->t_io);
  printf("Deadline: %d\n", p->t_deadline);
  printf("Finish Time: %d\n", p->t_finish);
  printf("Number of Interrupts: %d\n", p->n_interrupts);
  printf("Time left in CPU: %d\n", p->t_lcpu);
  printf("Waiting Time: %d\n", p->waiting_time);
  printf("Turnaround Time: %d\n", p->turnaround_time);
  printf("Sum of Deadlines: %d\n", p->sum_deadline);
}



// Function to write a csv file with the results
void write_csv(Process* processes, int len, char* file_name) {
  FILE* file = fopen(file_name, "w");
  if (!file) {
    printf("Error opening the file\n");
    return;
  }

  for (int i = 0; i < len; ++i) {
    int response_time = processes[i].first_execution_time - processes[i].t_start; 

    // name, pid, n_interrupts, turnaround_time, response_time, waiting_time, sum_deadline
    fprintf(file, "%s,%d,%d,%d,%d,%d,%d\n", processes[i].name, processes[i].pid, processes[i].n_interrupts, processes[i].turnaround_time, response_time, processes[i].waiting_time, processes[i].sum_deadline);
  }

  fclose(file);
}
