#if ! defined ( PCB_H )
#define PCB_H

// PCB DECLARATIOION

typedef enum PROCESS_STATE
{
	NEW, READY, RUNNING, WAITING, TERMINATED
} PROCESS_STATE;

typedef struct PCB{
	int pid;
	int arrivalTime;
	int totalCPUTime;
	int IOFrequency;
	int IODuration;
	PROCESS_STATE state;
} PCB;

PCB* createPCB(int pid,
	int arrivalTime,
	int totalCPUTime,
	int IOFrequency,
	int IODuration);

void deletePCB(PCB* pcb);

void printPCB(PCB* pcb);


// PCB QUEUE DECLARATION

typedef struct PCBQueue{
	PCB** PCBs;
	int MAX;
	int front;
	int rear;
	int itemCount;
} PCBQueue;


PCBQueue* PCBQCreate(int max);

void PCBQDelete(PCBQueue* queue);

PCB* PCBQFront(const PCBQueue* queue); // return the first element

int PCBQisEmpty(const PCBQueue* queue);

int PCBQisFull(const PCBQueue* queue);

int PCBQSize(const PCBQueue* queue);

void PCBQInsert(PCBQueue* queue, PCB* data);

PCB* PCBQPop(PCBQueue* queue);


#endif // PCB_H