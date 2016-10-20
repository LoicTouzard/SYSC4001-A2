#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"


// PCB IMPLEMENTATION

PCB* PCBCreate(int pid,
	int arrivalTime,
	int totalCPUTime,
	int IOFrequency,
	int IODuration)
{
	PCB* pcb = (PCB*)malloc(sizeof(PCB));
	pcb->pid = pid;
	pcb->arrivalTime = arrivalTime;
	pcb->totalCPUTime = totalCPUTime;
	pcb->remainingCPUTime = 0;
	pcb->IOFrequency = IOFrequency;
	pcb->IODuration = IODuration;
	pcb->state = UNDEFINED;
	return pcb;
}


void PCBDelete(PCB* pcb)
{
	free(pcb);
}

void PCBPrint(PCB* pcb)
{

	printf("Process %d : arrivalTime %d | totalCPUTime %d | IOFrequency %d | IODuration %d | state %s\n",
		pcb->pid, pcb->arrivalTime, pcb->totalCPUTime, pcb->IOFrequency, pcb->IODuration,
			(pcb->state==NEW?"NEW":
				(pcb->state==READY?"READY":
					(pcb->state==RUNNING?"RUNNING":
						(pcb->state==WAITING?"WAITING":
							(pcb->state==TERMINATED?"TERMINATED":"UNKNOWN"
			))))));
}

int PCBCmp(const void * a, const void * b)
{
   return ( (*(PCB**)a)->arrivalTime - (*(PCB**)b)->arrivalTime );
}


// PCB QUEUE IMPLEMENTATION

PCBQueue* PCBQCreate(int max)
{
	if(max <= 0) return NULL;
	PCBQueue* pcbq = (PCBQueue*)malloc(sizeof(PCBQueue));
	pcbq->PCBs = (PCB**)malloc(max * sizeof(PCB*));
	pcbq->MAX = max;
	pcbq->front = 0;
	pcbq->rear = -1;
	pcbq->itemCount = 0;
	return pcbq;
}

void PCBQDelete(PCBQueue* queue)
{
	free(queue->PCBs);
	free(queue);
}

PCB* PCBQFront(const PCBQueue* queue)
{
   return queue->PCBs[queue->front];
}

int PCBQisEmpty(const PCBQueue* queue)
{
   return queue->itemCount == 0;
}

int PCBQisFull(const PCBQueue* queue)
{
   return queue->itemCount == queue->MAX;
}

int PCBQSize(const PCBQueue* queue)
{
	return queue->itemCount;
}  

void PCBQInsert(PCBQueue* queue, PCB* data)
{
	if(!PCBQisFull(queue))
	{	
		if(queue->rear == queue->MAX-1)
		{
			queue->rear = -1;            
		}
		queue->PCBs[++(queue->rear)] = data;
		queue->itemCount++;
	}
}

PCB* PCBQPop(PCBQueue* queue)
{
	PCB* data = queue->PCBs[queue->front++];

	if(queue->front == queue->MAX)
	{
		queue->front = 0;
	}

	queue->itemCount--;
	return data;  
}