#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <stdbool.h>
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"


void free_processes_array(Process* process_array, int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        // Liberar las cadenas de caracteres dentro de cada proceso
        if (process_array[i].name != NULL) {
            free(process_array[i].name);
        }
        if (process_array[i].state != NULL) {
            free(process_array[i].state);
        }
    }
    // Liberar el arreglo de procesos
    free(process_array);
}

// Función para liberar la cola
void free_queue(Queue* q) {
    if (q != NULL) {
        // Si el arreglo de procesos en la cola fue asignado dinámicamente, lo liberamos
        if (q->processes != NULL) {
            free(q->processes);  // Liberar el arreglo de procesos
        }

        // Finalmente, liberar la estructura `Queue`
        free(q);
    }
}

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	char *output_file = (char *)argv[2];
	char *quantum = (char *)argv[3];

	InputFile *input_file = read_file(file_name);

	//transformar quantum a int
	int int_quantum = atoi(quantum);

	// Creamos las colas de procesos high y low high con el quantum x 2
	Queue *high_priority_queue = create_queue("High", int_quantum, input_file->len);
	Queue *low_priority_queue = create_queue("Low", int_quantum, input_file->len);

	// Arreglo para guardar los procesos
	// Process *processes[input_file->len];

	Process* processes;
	processes = calloc(input_file->len, sizeof(Process));

	for (int i = 0; i < input_file->len; ++i)
	{
		// Creamos el proceso
		Process *p = create_process(input_file->lines[i][0], atoi(input_file->lines[i][1]), atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), atoi(input_file->lines[i][4]), atoi(input_file->lines[i][5]), atoi(input_file->lines[i][6]));
		processes[i] = *p;
	}

	// Iteramos sobre los ticks de ejecución del programa
	int tick = 0;
	bool all_finished = false;

	while (!all_finished)
	{
		// Verificamos si todos los procesos han terminado
		all_finished = true;
		for (int i = 0; i < input_file->len; ++i)
		{
			if (processes[i].t_finish == 0)
			{
				all_finished = false;
				break;
			}
		}

		// Iteramos sobre los procesos
		for (int i = 0; i < input_file->len; ++i)
		{
			// Verificamos si el proceso ha terminado
			if (processes[i].t_finish == 0) // no ha terminado
			{
				// Verificamos si el proceso ha llegado al tick de inicio
				if (processes[i].t_start == tick)
				{
					// Agregamos el proceso a la cola high
					enqueue(high_priority_queue, &processes[i]);
				}
			}
		}

		//definimos la prioridad de los procesos
		set_priority(tick, high_priority_queue);
		set_priority(tick, low_priority_queue);

		//ordenamos las colas
		sort_queue(high_priority_queue);
		sort_queue(low_priority_queue);
	

		// Iteramos sobre las colas
		iterate_queues(high_priority_queue, low_priority_queue, tick);



		// Incrementamos el tick
		tick++;

		// Salimos del ciclo si ya pasaron 10 ticks, es solo para probar
		if (all_finished) {
			print_queue(high_priority_queue);
			write_csv(processes, input_file->len, output_file);
			//imprimir los atributos de un proceso
			free_processes_array(processes, input_file->len);
			free_queue(high_priority_queue);
			free_queue(low_priority_queue);
			break;
		}
	}



	input_file_destroy(input_file);
}



