#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OutputFileManager.h"
#include "PCB.h"


static FILE* outputFile = NULL; // output file pointer

int OFMOpen(const char* filename)
{
	outputFile = fopen(filename, "w"); // open the file and store the pointer
	if(outputFile == NULL)
	{
		printf("Failed to open the output file : %s\n", filename);
		return -1;
	}
	return 0;
}

int OFMClose()
{
	int closed = fclose(outputFile); // close the stored file pointer
	outputFile = NULL;
	if(closed != 0)
	{
		printf("Failed to close the output file\n");
		return -1;
	}
	return 0;
}


int OFMLogState(int time, PCB* process, PROCESS_STATE newState)
{
	if(outputFile != NULL){
		return fprintf(outputFile, "%d;%d;%s;%s\n", time, process->pid, stateToStr(process->state), stateToStr(newState));
	}
	else{
		return -1;
	}
}
