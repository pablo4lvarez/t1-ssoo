#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Function to create a new queue
Queue* create_queue(const char* name, int quantum, int capacity) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) return NULL;

    strcpy(q->name, name);
    q->quantum = quantum;
    q->capacity = capacity;
    q->size = 0;
    q->processes = (Process**)malloc(capacity * sizeof(Process*));

    if (!q->processes) {
        free(q);
        return NULL;
    }

    return q;
}

// Verificar si la cola está vacía
int is_empty(Queue* q) {
    return q->size == 0;
}

// Encolar un proceso
void enqueue(Queue* q, Process* p) {
    if (q->size < q->capacity) {
        q->processes[q->size] = p;
        q->size++;
    } else {
        printf("La cola está llena, no se puede agregar más procesos\n");
    }
}

// Desencolar un proceso
Process* dequeue(Queue* q) {
    if (is_empty(q)) {
        printf("La cola está vacía\n");
        return NULL;
    }

    Process* p = q->processes[0];

    // Mover todos los procesos hacia adelante
    for (int i = 1; i < q->size; i++) {
        q->processes[i - 1] = q->processes[i];
    }
    q->size--;

    return p;
}

// Imprimir el contenido de la cola
void print_queue(Queue* q) {
    printf("Cola %s (Quantum: %d) - Procesos:\n", q->name, q->quantum);
    for (int i = 0; i < q->size; i++) {
        printf("Proceso %s (PID: %d)\n", q->processes[i]->name, q->processes[i]->pid);
    }
}