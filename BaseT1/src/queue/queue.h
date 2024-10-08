#pragma once

#include <sys/types.h>
#include "../process/process.h"

typedef struct {
  char name[50];        // Nombre de la cola
  Process** processes;   // Arreglo dinámico de procesos
  int capacity;          // Capacidad máxima de la cola
  int size;              // Número actual de procesos en la cola
  int quantum;           // Quantum de la cola
} Queue;


Queue* create_queue(char* name, int quantum, int capacity);

int is_empty(Queue* q);

void enqueue(Queue* q, Process* p);

Process* dequeue(Queue* q);

void sort_queue(Queue* q);

void set_priority(int tick, Queue* q);

void print_queue(Queue* q);

void iterate_queues(Queue* q1, Queue* q2, int tick);
