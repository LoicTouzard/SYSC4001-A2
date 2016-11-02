#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InputFileManager.h"
#include "PCB.h"


static FILE* inputFile = NULL; // input file pointer

int IFMOpen(const char* filename)
{
	inputFile = fopen(filename, "r"); // open the file and store the pointer
	if(inputFile == NULL)
	{
		printf("Failed to open the input file : %s\n", filename);
		return -1;
	}
	return 0;
}

int IFMClose()
{
	int closed = fclose(inputFile); // close the stored file pointer
	inputFile = NULL;
	if(closed != 0)
	{
		printf("Failed to close the input file\n");
		return -1;
	}
	return 0;
}


// Parse a read line into a process structure
PCB* lineToProcess(char* line)
{
    // read all the process caracteristics
	const char* pid = strtok (line,";");
	const char* arrivalTime = strtok(NULL, ";");
	const char* totalCPUTime = strtok(NULL, ";");
	const char* IOFrequency = strtok(NULL, ";");
	const char* IODuration = strtok(NULL, ";");
	const char* priority = strtok(NULL, ";");

	if(pid == NULL || arrivalTime == NULL || totalCPUTime == NULL ||IOFrequency == NULL || IODuration == NULL)
	{
		// something was missing, abort line
		return NULL;
	}
	// dynamically create the process in memory
	PCB* process = PCBCreate(atoi(pid),
		atoi(arrivalTime),
		atoi(totalCPUTime),
		atoi(IOFrequency),
		atoi(IODuration),
		((priority == NULL) ? 0 : atoi(priority)));// priority is optionnal, when not found is set to 0
	
	return process;
}

int IFMReadProcesses(PCB*** processes)
{
	rewind(inputFile);// reset file pointer, just in case
	*processes = (PCB**)malloc(1*sizeof(PCB*)); // create the PCB array beginning with size 1
	int size = 0;

	char line[128];

    while (fgets(line, 128, inputFile)) // while we can read a new line
    {
        char* tmp = strdup(line); // Note : strdup duplicate by allocating memory
        PCB* process = lineToProcess(tmp); // try to allocate a process from this line

        if(process != NULL)
        {
        	// add this new process to array, reallocating to adapt the size
        	size++;
    		*processes = (PCB**)realloc(*processes, size * sizeof(PCB*));
        	(*processes)[size-1] = process;
        }
        free(tmp);
    }

    return size;
}
