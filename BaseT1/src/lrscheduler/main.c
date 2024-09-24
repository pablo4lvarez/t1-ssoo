#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <stdbool.h>
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	char *output_file = (char *)argv[2];
	char *quantum = (char *)argv[3];

	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);

	// Creamos las colas de procesos high y low
	Queue *high_priority_queue = create_queue("High", quantum, input_file->len);
	Queue *low_priority_queue = create_queue("Low", quantum, input_file->len);

	// Arreglo para guardar los procesos
	// Process *processes[input_file->len];

	Process* processes;
	processes = calloc(input_file->len, sizeof(Process));

	printf("Procesos:\n");
	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("%s ", input_file->lines[i][j]);
		}
		printf("\n");
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
					print_queue(high_priority_queue);
				}
			}
		}



		// Incrementamos el tick
		tick++;

		// Salimos del ciclo si ya pasaron 10 ticks, es solo para probar
		if (tick == 10) {
			print_queue(high_priority_queue);
			// write_csv(processes, input_file->len, output_file);
			break;
		}
	}



	input_file_destroy(input_file);
}
