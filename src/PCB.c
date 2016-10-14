#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"

PCB* createPCB(int pid,
	int arrivalTime,
	int totalCPUTime,
	int IOFrequency,
	int IODuration)
{
	PCB* pcb = (PCB*)malloc(sizeof(PCB));
	pcb->pid = pid;
	pcb->arrivalTime = arrivalTime;
	pcb->totalCPUTime = totalCPUTime;
	pcb->IOFrequency = IOFrequency;
	pcb->IODuration = IODuration;
	pcb->state = NEW;
	return pcb;
}


void deletePCB(PCB* pcb)
{
	free(pcb);
}

void printPCB(PCB* pcb)
{
	printf("Process %d : arrivalTime %d | totalCPUTime %d | IOFrequency %d | IODuration %d | state %d\n",
		pcb->pid, pcb->arrivalTime, pcb->totalCPUTime, pcb->IOFrequency, pcb->IODuration, pcb->state);
}