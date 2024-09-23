#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Function to create a new queue
Queue* create_queue(int quantum) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->quantum = quantum;
    return q;
}

// Function to check if the queue is empty
int is_empty(Queue* q) {
    return q->front == NULL;
}

// Function to add a process to the queue
void enqueue(Queue* q, Process* p) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->process = p;
    new_node->next = NULL;
    if (is_empty(q)) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

// Function to remove a process from the queue
Process* dequeue(Queue* q) {
    if (is_empty(q)) {
        return NULL;
    }
    Node* temp = q->front;
    Process* p = temp->process;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return p;
}

// Function to print the queue
void print_queue(Queue* q) {
    Node* current = q->front;
    while (current != NULL) {
        print_process(current->process);
        current = current->next;
    }
}

// Function to calculate quantum
int calculate_quantum(int q, char* queue_type) {
    if (queue_type == "High") {
        return 2 * q;
    } else if (queue_type == "Low") {
        return q;
    }
    return -1; // Error en caso de tipo de cola inválido
}