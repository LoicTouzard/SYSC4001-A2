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

void printHelp()
{
	printf("Use : ./Simulator <input_file> <output_file> <ALGORITHM> <VERBOSE>\n");
	printf("\t<ALGORITHM> can be 'FCFS', 'PRIORITY', 'SJF'.\n");
	printf("\t<VERBOSE>   (optional) can be 'TRACE', 'GANTT'. Default gives none.\n");
}

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		printHelp();
		return EXIT_FAILURE;
	}

	if(IFMOpen(argv[1]) == -1) return EXIT_FAILURE;

	PCB** processes = NULL;
	// read and create all the processes from input
	int nbProcesses = IFMReadProcesses(&processes);

	if(IFMClose() == -1) return EXIT_FAILURE;

	printAllProcesses(processes, nbProcesses);

	if(OFMOpen(argv[2]) == -1) return EXIT_FAILURE;
	

	/* Simulation goes here */

	SimulatorSetProcesses(processes, nbProcesses);
	SimulatorSetMode(MODE_NONPREEMPTIVE);

	// ALGORITHM
	if(strcmp(argv[3], "SJF") == 0)
	{
		SimulatorSetAlgorithm(ALGORITHM_SJF);
	}
	else if(strcmp(argv[3], "PRIORITY") == 0)
	{
		SimulatorSetAlgorithm(ALGORITHM_PRIORITY);
	}
	else
	{
		SimulatorSetAlgorithm(ALGORITHM_FCFS);
	}

	// VERBOSE
	if(argc > 4)
	{
		if(strcmp(argv[4], "TRACE") == 0)
		{
			SimulatorSetVerbose(TRACE);
		}
		else if(strcmp(argv[4], "GANTT") == 0)
		{
			SimulatorSetVerbose(GANTT);
		}
	}

	SimulatorRun();// run the simulation until its end
	SimulatorPrintStats();


	/* end of simulation */


	if(OFMClose() == -1) return EXIT_FAILURE;

	// free processes data
	for (int i = 0; i < nbProcesses; ++i)
	{
		PCBDelete(processes[i]);
	}
	free(processes);


	return EXIT_SUCCESS;
}