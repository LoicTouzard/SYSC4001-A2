#if ! defined ( PCBQUEUE_H )
#define PCBQUEUE_H

#include "PCB.h"

/** PCB QUEUE DECLARATION **/

// Queue data structure for PCB*
// Use the methods to manipulate the queue, DO NOT USE the struct directly
typedef struct PCBQueue{
	PCB** PCBs; 	// array of PCB, used as queue
	int MAX;		// MAX size of the queue
	int front;		// index to the front of the queue
	int rear;		// index for inserting new data
	int itemCount;	// number of items in the queue
} PCBQueue;


// Create dynamically and return a PCBQueue having the parameter max as size
PCBQueue* PCBQCreate(int max);

// Free a Queue
// NOTE : Doens't free the content of it.
void PCBQDelete(PCBQueue* queue);

// Return the first element of the queue
PCB* PCBQFront(const PCBQueue* queue);

// Return 1 if the queue is empty, else 0
int PCBQisEmpty(const PCBQueue* queue);

// Return 1 if the queue is full, else 0
int PCBQisFull(const PCBQueue* queue);

// Return the size of the queue
int PCBQSize(const PCBQueue* queue);

// Insert a new PCB to the end of the queue
void PCBQInsert(PCBQueue* queue, PCB* data);

// return the first element and delete it from the queue
PCB* PCBQPop(PCBQueue* queue);


#endif // PCBQUEUE_H