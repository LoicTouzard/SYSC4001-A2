#if ! defined ( PCB_H )
#define PCB_H

// PCB DECLARATIOION

typedef enum PROCESS_STATE
{
	UNDEFINED, NEW, READY, RUNNING, WAITING, TERMINATED
} PROCESS_STATE;

char* stateToStr(PROCESS_STATE);

typedef struct PCB{
	int pid;							// pid, from input
	int arrivalTime;					// arrival time in system, from input
	int totalCPUTime;					// total CPU Time required for the task, from input
	int totalCPUTimeUsed;				// total CPU time used already
	int remainingCPUTime;				// remaining CPU Time before preemption
	int IOFrequency;					// IO frequency, from input
	int IODuration;						// IO duration, from input
	int remainingBeforeIORequestTime;	// Remaining time before the next IO request
	int remainingIODuration;			// Time remaining with IO in WAITING state
	int waitTime;						// time spent in the ready queue
	PROCESS_STATE state;				// process state
} PCB;

PCB* PCBCreate(int pid,
	int arrivalTime,
	int totalCPUTime,
	int IOFrequency,
	int IODuration);

void PCBDelete(PCB* pcb);

void PCBPrint(PCB* pcb);

int PCBCmp(const void * a, const void * b);

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