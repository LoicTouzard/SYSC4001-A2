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
	// read and create all the processes from input
	int nbProcesses = IFMReadProcesses(&processes);

	if(IFMClose() == -1) return EXIT_FAILURE;

	printAllProcesses(processes, nbProcesses);

	if(OFMOpen(argv[2]) == -1) return EXIT_FAILURE;
	

	/* Simulation goes here */
	SimulatorSetVerbose(TRACE);
	SimulatorSetProcesses(processes, nbProcesses);
	SimulatorSetMode(MODE_NONPREEMPTIVE);

	SimulatorSetAlgorithm(ALGORITHM_SJF);
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