#pragma once

#include <sys/types.h>
#include "../process/process.h"

typedef struct Node {
  Process* process;
  struct Node* next;
} Node;

typedef struct {
  Node* front;
  Node* rear;
  int quantum;
} Queue;

Queue* create_queue(int quantum);

int is_empty(Queue* q);

void enqueue(Queue* q, Process* p);

Process* dequeue(Queue* q);

void print_queue(Queue* q);

int calculate_quantum(int q, char* queue_type);
