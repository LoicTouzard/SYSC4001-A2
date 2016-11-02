#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "PCB.h"
#include "PCBQueue.h"

// PCB QUEUE IMPLEMENTATION

PCBQueue* PCBQCreate(int max)
{
	PCBQueue* pcbq = (PCBQueue*)malloc(sizeof(PCBQueue));
	if(max <= 0)
	{
		max = 0;
	}
	else
	{
		pcbq->PCBs = (PCB**)malloc(max * sizeof(PCB*));
	}
	pcbq->MAX = max;
	pcbq->front = 0;
	pcbq->rear = -1;
	pcbq->itemCount = 0;
	return pcbq;
}

void PCBQDelete(PCBQueue* queue)
{
	// free the inner array before freeing the queue
	if(queue->MAX != 0) free(queue->PCBs);
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
		if(queue->rear == queue->MAX-1) // index cycle if at the end
		{
			queue->rear = -1;            
		}
		queue->PCBs[++(queue->rear)] = data; // insertion
		queue->itemCount++;
	}
}

PCB* PCBQPop(PCBQueue* queue)
{
	PCB* data = queue->PCBs[queue->front++];

	if(queue->front == queue->MAX) // index cycle if at the end
	{
		queue->front = 0;
	}

	queue->itemCount--;
	return data;  
}