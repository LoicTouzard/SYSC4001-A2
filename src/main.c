#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InputFileManager.h"
#include "PCB.h"

void printAllProcesses(const PCB** processes, int nbProcesses)
{
	for (int i = 0; i < nbProcesses; ++i) PCBPrint(processes[i]);
}

int main(int argc, char* argv[])
{
	if(argc != 3){
		printf("Use : ./Simulator <input_file> <output_file>\n");
		return EXIT_FAILURE;
	}

	if(IFMOpen(argv[1]) == -1) return EXIT_FAILURE;

	PCB** processes = NULL;
	int nbProcesses = IFMReadProcesses(&processes);
	printf("%d Process%s created\n", nbProcesses, (nbProcesses>1?"es":""));

	// Sort PCBs by Arrival Time
	qsort(processes, nbProcesses, sizeof(PCB*), PCBCmp);

	printAllProcesses(processes, nbProcesses);


	// execution goes here






	for (int i = 0; i < nbProcesses; ++i)
	{
		PCBDelete(processes[i]);
	}
	free(processes);


	if(IFMClose() == -1) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}