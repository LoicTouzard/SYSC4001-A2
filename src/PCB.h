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
	int priority;						// the priority of the process low is 0, then in order of positive integers
	PROCESS_STATE state;				// process current state
} PCB;

// PCB constructor
PCB* PCBCreate(int pid,
	int arrivalTime,
	int totalCPUTime,
	int IOFrequency,
	int IODuration,
	int priority);

// reset the caracteristics of the PCB due to simulation
void PCBReset(PCB* pcb);

// free a PCB struct
void PCBDelete(PCB* pcb);

// print information from input about the given PCB
void PCBPrint(PCB* pcb);

// Comparaison function between two PCB for qsort, it uses the arrivalTime discriminator
int PCBCmp(const void * a, const void * b);


#endif // PCB_H