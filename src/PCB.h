#if ! defined ( PCB_H )
#define PCB_H

/** PCB DECLARATIONS **/

// enum for the different process states
typedef enum PROCESS_STATE
{
	UNDEFINED, NEW, READY, RUNNING, WAITING, TERMINATED
} PROCESS_STATE;

// convert a PROCESS_STATE value into it's name as c-string
char* stateToStr(PROCESS_STATE);

// PCB Structure
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
	PROCESS_STATE state;				// process current state
} PCB;

// PCB constructor
PCB* PCBCreate(int pid,
	int arrivalTime,
	int totalCPUTime,
	int IOFrequency,
	int IODuration);

// reset the caracteristics of the PCB due to simulation
void PCBReset(PCB* pcb);

// free a PCB struct
void PCBDelete(PCB* pcb);

// print information from input about the given PCB
void PCBPrint(PCB* pcb);

// Comparaison function between two PCB for qsort, it uses the arrivalTime discriminator
int PCBCmp(const void * a, const void * b);



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


#endif // PCB_H