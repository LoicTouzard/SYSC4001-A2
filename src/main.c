#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InputFileManager.h"
#include "OutputFileManager.h"
#include "PCB.h"
#include "Simulator.h"



void printAllProcesses(PCB** processes, int nbProcesses)
{
	for (int i = 0; i < nbProcesses; ++i) PCBPrint(processes[i]);
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Use : ./Simulator <input_file> <output_file>\n");
		return EXIT_FAILURE;
	}

	if(IFMOpen(argv[1]) == -1) return EXIT_FAILURE;

	PCB** processes = NULL;
	int nbProcesses = IFMReadProcesses(&processes);
	printf("%d Process%s created from input\n", nbProcesses, (nbProcesses>1?"es":""));

	if(IFMClose() == -1) return EXIT_FAILURE;

	// Sort PCBs by Arrival Time
	qsort(processes, nbProcesses, sizeof(PCB*), PCBCmp);

	printAllProcesses(processes, nbProcesses);


	if(OFMOpen(argv[2]) == -1) return EXIT_FAILURE;
	
	// Simulation goes here
	SimulatorSetProcesses(processes, nbProcesses);
	SimulatorSetMode(MODE_NONPREEMPTIVE);
	SimulatorSetAlgorithm(ALGORITHM_FCFS);
	SimulatorRun();

	if(OFMClose() == -1) return EXIT_FAILURE;

	// free processes data
	for (int i = 0; i < nbProcesses; ++i)
	{
		PCBDelete(processes[i]);
	}
	free(processes);


	return EXIT_SUCCESS;
}