#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InputFileManager.h"
#include "PCB.h"

static FILE* inputFile = NULL;

int IFMOpen(const char* filename)
{
	
	inputFile = fopen(filename, "r");
	if(inputFile == NULL)
	{
		printf("Failed to open the input file : %s\n", filename);
		return -1;
	}
	return 0;
}

int IFMClose()
{
	int closed = fclose(inputFile);
	inputFile = NULL;
	if(closed != 0)
	{
		printf("Failed to close the input file\n");
		return -1;
	}
	return 0;
}


PCB* lineToProcess(char* line)
{
    // read the process caracteristics
	const char* pid = strtok (line,";");
	const char* arrivalTime = strtok(NULL, ";");
	const char* totalCPUTime = strtok(NULL, ";");
	const char* IOFrequency = strtok(NULL, ";");
	const char* IODuration = strtok(NULL, ";");

	if(pid == NULL || arrivalTime == NULL || totalCPUTime == NULL ||IOFrequency == NULL || IODuration == NULL)
	{
		// something was missing
		return NULL;
	}
	PCB* process = PCBCreate(atoi(pid), atoi(arrivalTime), atoi(totalCPUTime), atoi(IOFrequency), atoi(IODuration));
	
	return process;
}

int IFMReadProcesses(PCB*** processes)
{
	rewind(inputFile);
	*processes = (PCB**)malloc(1*sizeof(PCB*));
	int size = 0;

	char line[128];

    while (fgets(line, 128, inputFile))
    {
        char* tmp = strdup(line); // duplicate allocates memory
        PCB* process = lineToProcess(tmp);

        if(process != NULL)
        {
        	size++;
    		*processes = (PCB**)realloc(*processes, size * sizeof(PCB*));
        	(*processes)[size-1] = process;
        }
        free(tmp);
    }

    return size;
}
