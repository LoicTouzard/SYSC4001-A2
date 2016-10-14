#if ! defined ( PCB_H )
#define PCB_H

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
#endif // PCB_H