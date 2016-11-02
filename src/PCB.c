#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "PCB.h"


char* stateToStr(PROCESS_STATE state)
{
	return (state==NEW?"NEW":
		(state==READY?"READY":
			(state==RUNNING?"RUNNING":
				(state==WAITING?"WAITING":
					(state==TERMINATED?"TERMINATED":"UNKNOWN"
	)))));
}

// PCB IMPLEMENTATION

PCB* PCBCreate(int pid,
	int arrivalTime,
	int totalCPUTime,
	int IOFrequency,
	int IODuration,
	int priority)
{
	// allocate a new PCB
	PCB* pcb = (PCB*)malloc(sizeof(PCB));
	// set all the values (checking if some value are < 0 too)
	pcb->pid = pid;
	pcb->arrivalTime = (arrivalTime < 0)?0:arrivalTime;
	pcb->totalCPUTime = (totalCPUTime < 0)?0:totalCPUTime;
	pcb->totalCPUTimeUsed = 0;
	pcb->remainingCPUTime = 0;
	pcb->IOFrequency = (IOFrequency < 0)?0:IOFrequency;
	pcb->IODuration = (IODuration < 0)?0:IODuration;
	pcb->remainingBeforeIORequestTime = (IOFrequency <= 0)?-1:IOFrequency;// IOFrequency to 0 results in no IO interruption
	pcb->remainingIODuration = 0;
	pcb->waitTime = 0;
	pcb->priority = (priority < 0)?0:priority;
	pcb->state = UNDEFINED;
	return pcb;
}

void PCBReset(PCB* pcb)
{
	// reset all the values modified in simulation
	pcb->totalCPUTimeUsed = 0;
	pcb->remainingCPUTime = 0;
	pcb->remainingBeforeIORequestTime = (pcb->IOFrequency <= 0)?-1:pcb->IOFrequency;// IOFrequency to 0 results in no IO interruption
	pcb->remainingIODuration = 0;
	pcb->waitTime = 0;
	pcb->state = UNDEFINED;
}

void PCBDelete(PCB* pcb)
{
	free(pcb);
}

void PCBPrint(PCB* pcb)
{
	// only info from input (was for parsing debug)
	printf("Process %d : arrivalTime %d | totalCPUTime %d | IOFrequency %d | IODuration %d | priority %d | state %s\n",
		pcb->pid, pcb->arrivalTime, pcb->totalCPUTime, pcb->IOFrequency, pcb->IODuration, pcb->priority, stateToStr(pcb->state));
}

int PCBCmp(const void * a, const void * b)
{
   return ( (*(PCB**)a)->arrivalTime - (*(PCB**)b)->arrivalTime );
}