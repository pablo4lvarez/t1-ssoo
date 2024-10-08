#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

// Function to create a new queue
Queue* create_queue(char* name, int int_quantum, int capacity) {
    Queue* q = (Queue*)calloc(1, sizeof(Queue));
    if (!q) return NULL;

    strcpy(q->name, name);

    if (strcmp(name, "High") == 0) {
        q->quantum = 2 * int_quantum;
    } else if (strcmp(name, "Low") == 0) {
        q->quantum = int_quantum;
    } else {
        printf("Tipo de cola no válido. Usando el valor por defecto.\n");
        q->quantum = int_quantum;      // Valor por defecto en caso de un tipo inválido
    }

    // Inicializar los demás campos
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
        p->t_lcpu = q->quantum;
        q->processes[q->size] = p;
        q->size++;
    } else {
        printf("La cola está llena, no se puede agregar más procesos\n");
    }
}

// Desencolar el porceso p de la cola q
void dequeue_process(Queue* q, Process* p) {
    for (int i = 0; i < q->size; i++) {
        if (q->processes[i]->pid == p->pid) {
            for (int j = i; j < q->size - 1; j++) {
                q->processes[j] = q->processes[j + 1];
            }
            q->size--;
            break;
        }
    }
}

//Ordenar por prioridad invertida
void sort_queue(Queue* q) {
    for (int i = 0; i < q->size - 1; i++) {
        for (int j = i + 1; j < q->size; j++) {
            if (q->processes[i]->priority < q->processes[j]->priority) {
                Process* temp = q->processes[i];
                q->processes[i] = q->processes[j];
                q->processes[j] = temp;
            }
            else if (q->processes[i]->priority == q->processes[j]->priority) {
                if (q->processes[i]->pid < q->processes[j]->pid) {
                    Process* temp = q->processes[i];
                    q->processes[i] = q->processes[j];
                    q->processes[j] = temp;
                }
            }
        }
    }
}

// Determinar priorodad de un proceso
void set_priority(int tick, Queue* q) {
    for (int i = 0; i < q->size - 1; i++) {
        if (strcmp(q->processes[i]->state, "READY") == 0) {
            q->processes[i]->priority = tick - q->processes[i]->t_lcpu - q->processes[i]->t_deadline;
        } else if (strcmp(q->processes[i]->state, "RUNNING") == 0) {
            q->processes[i]->priority = 999999999;
        }
    }
}

// mover un proceso de cola origen a cola destino
void move_process(Queue* q1, Queue* q2, Process* p) {
    p->t_lcpu = q2->quantum;
    enqueue(q2, p);
    dequeue_process(q1, p);
}

// Imprimir el contenido de la cola
void print_queue(Queue* q) {
    printf("Cola %s (Quantum: %d) - Procesos:\n", q->name, q->quantum);
    for (int i = 0; i < q->size; i++) {
        printf("Proceso %s (PID: %d)\n", q->processes[i]->name, q->processes[i]->pid);
    }
}

// Iterar sobre ambas colas
// q1 es la cola de alta prioridad y q2 la de baja prioridad
void iterate_queues(Queue* q1, Queue* q2, int tick) {
    //imprimir el tick actual
    printf("Tick: %d\n", tick);
    if (q1->size == 0 && q2->size == 0) {
        printf("No hay procesos en ninguna cola\n");
        return;
    }
    //Si es que no hay ningun proceso RUNNING en q1, hacer que el primer proceso en READY pase a RUNNING
    if (q1->size > 0) {
        //iteramos sobre los procesos de la cola
        for (int i = 0; i < q1->size; i++) {
            if (strcmp(q1->processes[i]->state, "RUNNING") == 0) {
                break;
            }
            if (strcmp(q1->processes[i]->state, "READY") == 0) {
                q1->processes[i]->state = "RUNNING";
                break;
            }
        }
    } else if (q2->size > 0) {
        //iteramos sobre los procesos de la cola
        for (int i = 0; i < q2->size; i++) {
            if (strcmp(q2->processes[i]->state, "RUNNING") == 0) {
                break;
            }
            if (strcmp(q2->processes[i]->state, "READY") == 0) {
                q2->processes[i]->state = "RUNNING";
                break;
            }
        }
    }

    for (int i = 0; i < q1->size; i++) {
        // imprimir el nombre del proceso con su estado
        printf("Proceso %s %s\n", q1->processes[i]->name, q1->processes[i]->state);
        if (strcmp(q1->processes[i]->state, "WAITING") == 0) {
            q1->processes[i]->waiting_time += 1;
            q1->processes[i]->current_state_time += 1;
            if (q1->processes[i]->current_state_time == q1->processes[i]->t_io) {
                q1->processes[i]->state = "READY";
                q1->processes[i]->current_state_time = 0;
            }
        } else if (strcmp(q1->processes[i]->state, "READY") == 0) {
            q1->processes[i]->waiting_time += 1;
        } else if (strcmp(q1->processes[i]->state, "RUNNING") == 0) {
            // Cambiar el valor de su primera ejecución en caso de que sea asi
            if (q1->processes[i]->first_execution_time == -1) {
                q1->processes[i]->first_execution_time = tick;
            }

            // Ver si es que el proceso está pasado de su deadline
            // if (tick - q1->processes[i]->t_deadline - q1 -> processes[i] -> t_lcpu > 0) {
            //     q1->processes[i]->sum_deadline += 1;
            // }

            q1->processes[i]->t_lcpu -= 1;
            q1->processes[i]->current_state_time += 1;
            // primero checkeamos si completó el burst o no
            if (q1->processes[i]->t_cpu_burst == q1->processes[i]->current_state_time) {
                // completó el burst
                q1->processes[i]->n_burst -= 1; // decrementamos el número de bursts que le quedan
                q1->processes[i]->current_state_time = 0;
                
                // ver si el proceso termino o no
                if (q1->processes[i]->n_burst == 0) {
                    // terminó
                    q1->processes[i]->state = "FINISHED";
                    q1->processes[i]->t_finish = tick; // NECESITAMOS EL TICK EN EL QUE TERMINA
                    q1->processes[i]->turnaround_time = q1->processes[i]->t_finish - q1->processes[i]->t_start;
                    // calcular turnaround time
                    q1->processes[i]->turnaround_time = q1->processes[i]->t_finish - q1->processes[i]->t_start;
                    // Sacarlo de la cola
                    dequeue_process(q1, q1->processes[i]);
                } else if (q1->processes[i]->t_lcpu == 0) {
                    // Terminó el quantum y el burst al mismo tiempo
                    q1->processes[i]->state = "WAITING";
                    q1->processes[i]->current_state_time = 0;
                    q1->processes[i]->t_lcpu = q1->quantum;

                } else {
                    // no terminó. Cedió el CPU, por ende el proceso se queda en la misma cola en estado WAITING
                    q1->processes[i]->state = "WAITING";
                }
            } else {
                // no completó el burst

                // ver si se le acabó el quantum
                if (q1->processes[i]->t_lcpu == 0) {
                    // se le acabó el quantum. Hay que moverlo a la cola de baja prioridad
                    q1->processes[i]->state = "READY";
                    q1->processes[i]->current_state_time = 0;
                    q1->processes[i]->n_interrupts += 1; // incrementamos el número de interrupciones
                    move_process(q1, q2, q1->processes[i]);
                } else {
                    // no se le acabó el quantum. El proceso sigue corriendo.
                    q1->processes[i]->state = "RUNNING";
                }
            }

                        // Ver si es que el proceso está pasado de su deadline
            if (tick - q1->processes[i]->t_deadline - q1 -> processes[i] -> t_lcpu > 0) {
                q1->processes[i]->sum_deadline += 1;
            }
        }
    }

    for (int i = 0; i < q2->size; i++) {
        //imprimir el nombre de la cola
        printf("Cola baja\n");
        printf("Proceso %s %s\n", q1->processes[i]->name, q1->processes[i]->state);
        // revisar si es posible mover algún proceso de la cola de baja prioridad a la de alta prioridad
        if (2 * q2->processes[i]->t_deadline < tick - q2->processes[i]->t_lcpu) {
            // mover proceso
            move_process(q2, q1, q2->processes[i]);
        }

        if (strcmp(q2->processes[i]->state, "WAITING") == 0) {
            q2->processes[i]->waiting_time += 1;
            q2->processes[i]->current_state_time += 1;
            if (q2->processes[i]->current_state_time == q2->processes[i]->t_io) {
                q2->processes[i]->state = "READY";
                q2->processes[i]->current_state_time = 0;
            }
        } else if (strcmp(q2->processes[i]->state, "READY") == 0) {
            q2->processes[i]->waiting_time += 1;
        } else if (strcmp(q2->processes[i]->state, "RUNNING") == 0) {
            // Cambiar el valor de su primera ejecución en caso de que sea asi
            if (q2->processes[i]->first_execution_time == -1) {
                q2->processes[i]->first_execution_time = tick;
            }

            // Ver si es que el proceso está pasado de su deadline
            if (tick - q2->processes[i]->t_deadline - q2 -> processes[i] -> t_lcpu > 0) {
                q2->processes[i]->sum_deadline += 1;
            }
            q2->processes[i]->t_lcpu -= 1;
            q2->processes[i]->current_state_time += 1;
            // primero checkeamos si completó el burst o no
            if (q2->processes[i]->t_cpu_burst == q2->processes[i]->current_state_time) {
                // completó el burst
                q2->processes[i]->n_burst -= 1; // decrementamos el número de bursts que le quedan
                
                // ver si el proceso termino o no
                if (q2->processes[i]->n_burst == 0) {
                    // terminó
                    q2->processes[i]->state = "FINISHED";
                    q2->processes[i]->t_finish = tick; // NECESITAMOS EL TICK EN EL QUE TERMINA
                    // calcular turnaround time
                    q2->processes[i]->turnaround_time = q2->processes[i]->t_finish - q2->processes[i]->t_start + 1;
                    // Sacarlo de la cola
                    dequeue_process(q2, q2->processes[i]);
                } else {
                    // no terminó Cedió el CPU, por ende el proceso se queda en la misma cola en estado WAITING
                    q2->processes[i]->state = "WAITING";
                    q2->processes[i]->current_state_time = 0;
                }
            } else {
                // no completó el burst

                // ver si se le acabó el quantum
                if (q2->processes[i]->t_lcpu == 0) {
                    // se le acabó el quantum.
                    q2->processes[i]->state = "READY";
                    q2->processes[i]->current_state_time = 0;
                    q2->processes[i]->n_interrupts += 1; // incrementamos el número de interrupciones
                    q2->processes[i]->t_lcpu = q2->quantum; // reseteamos el quantum
                } else {
                    // no se le acabó el quantum. El proceso sigue corriendo.
                    q2->processes[i]->state = "RUNNING";
                }
            }
        }
    }
}

