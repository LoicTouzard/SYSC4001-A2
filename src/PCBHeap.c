#include <stdio.h>
#include <stdlib.h>
 
typedef struct {
    int priority;
    char *data;
} node_t;

#include "PCB.h"
#include "PCBHeap.h"

// PCB HEAP (PRIORITY QUEUE) IMPLEMENTATION

PCBHeap* PCBHCreate(int max)
{
    PCBHeap* pcbh = (PCBHeap*)malloc(sizeof(PCBHeap));
    if(max <= 0)
    {
        max = 0;
    }
    else
    {
        pcbh->PCBs = (PCB**)malloc((max+1) * sizeof(PCB*));
    }
    pcbh->MAX = max;
    pcbh->len = 0;
    return pcbh;
}

void PCBHDelete(PCBHeap* heap)
{
    // free the inner array before freeing the heap
    if(heap->MAX != 0) free(heap->PCBs);
    free(heap);
}

PCB* PCBHFront(const PCBHeap* heap)
{
   return heap->PCBs[1];
}

int PCBHisEmpty(const PCBHeap* heap)
{
   return heap->len == 0;
}

int PCBHisFull(const PCBHeap* heap)
{
   return heap->len == heap->MAX;
}

int PCBHSize(const PCBHeap* heap)
{
    return heap->len;
}

void PCBHInsert(PCBHeap* heap, PCB* data)
{
    int i = heap->len + 1;// insertion index start at the end
    int j = i / 2; // 'parent' index
    while (i > 1 && heap->PCBs[j]->priority < data->priority) {// climb the flatten tree
        heap->PCBs[i] = heap->PCBs[j];
        i = j;
        j = j / 2; // go to parent
    }
    heap->PCBs[i] = data;
    heap->len++;
}

PCB* PCBHPop(PCBHeap* heap)
{
    int i, j, k;
    if (!heap->len) {
        return NULL;
    }
    PCB *data = heap->PCBs[1];

    //reorganise the heap
    heap->PCBs[1] = heap->PCBs[heap->len];
    heap->len--;
    i = 1;
    while (1) {
        k = i;
        j = 2 * i;
        if (j <= heap->len && heap->PCBs[j]->priority > heap->PCBs[k]->priority) {
            k = j;//switch with son
        }
        if (j + 1 <= heap->len && heap->PCBs[j + 1]->priority > heap->PCBs[k]->priority) {
            k = j + 1;//switch with neighbour
        }
        if (k == i) {
            break;
        }
        heap->PCBs[i] = heap->PCBs[k];// Do the switch  if the position changed
        i = k;// refresh the current position
    }
    heap->PCBs[i] = heap->PCBs[heap->len + 1];
    return data;
}