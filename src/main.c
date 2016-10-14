#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InputFileManager.h"
#include "PCB.h"


int main(int argc, char* argv[])
{
	if(argc != 3){
		printf("Use : ./Simulator <input_file> <output_file>\n");
		return EXIT_FAILURE;
	}

	if(openInputFile(argv[1]) == -1) return EXIT_FAILURE;

	PCB** processes = NULL;
	int nbProcesses = readProcesses(&processes);
	printf("%d Process%s created\n", nbProcesses, (nbProcesses>1?"es":""));

	for (int i = 0; i < nbProcesses; ++i)
	{
		printPCB(processes[i]);
		deletePCB(processes[i]);
	}
	free(processes);


	if(closeInputFile() == -1) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}