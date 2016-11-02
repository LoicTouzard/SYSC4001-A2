#if ! defined ( PCBHEAP_H )
#define PCBHEAP_H

#include "PCB.h"

/** PCB HEAP (PRIORITY QUEUE) DECLARATION **/

// Heap data structure for PCB*
// Use the methods to manipulate the queue, DO NOT USE the struct directly
typedef struct PCBHeap{
	PCB** PCBs; 	// array of PCB, used as queue
    int len;
    int MAX;
} PCBHeap;


// Create dynamically and return a PCBHeap having the parameter max as capacity
PCBHeap* PCBHCreate(int max);

// Free a heap
// NOTE : Doens't free the content of it just the structure
void PCBHDelete(PCBHeap* heap);

// Return the first element of the heap
PCB* PCBHFront(const PCBHeap* heap);

// Return 1 if the heap is empty, else 0
int PCBHisEmpty(const PCBHeap* heap);

// Return 1 if the heap is full, else 0
int PCBHisFull(const PCBHeap* heap);

// Return the size of the heap
int PCBHSize(const PCBHeap* heap);

// Insert a new PCB to the end of the heap
void PCBHInsert(PCBHeap* heap, PCB* data);

// return the first element and delete it from the heap
PCB* PCBHPop(PCBHeap* heap);


#endif // PCBHEAP_H