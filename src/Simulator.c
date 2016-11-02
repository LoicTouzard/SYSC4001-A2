#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Simulator.h"
#include "OutputFileManager.h"
#include "PCB.h"
#include "PCBQueue.h"
#include "PCBHeap.h"

char* modeToStr(SCHELDULER_MODE mode)
{
	return (mode==MODE_PREEMPTIVE?"PREEMPTIVE":
		(mode==MODE_NONPREEMPTIVE?"NONPREEMPTIVE":"UNKNOWN"
	));
}

char* algorithmToStr(SCHELDULER_ALGORITHM algorithm)
{
	return (algorithm==ALGORITHM_FCFS?"FCFS":
		(algorithm==ALGORITHM_PRIORITY?"PRIORITY":"UNKNOWN"
	));
}

static PCB** processes = NULL;
static int nbProcesses = 0;
static PCBQueue* newQueue = NULL;	// a queue containing the processes in NEW state
static PCBQueue* readyQueue = NULL;	// a queue containing the processes in READY state. Used for ALGORITHM_FCFS
static PCBHeap* readyHeap = NULL;	// a heap containing the processes in READY state. Used for ALGORITHM_PRIORITY
static PCBQueue* terminatedQueue = NULL;	// a queue containing the processes in TERMINATED state
static SCHELDULER_MODE MODE = MODE_NONPREEMPTIVE; // default
static SCHELDULER_ALGORITHM ALGORITHM = ALGORITHM_FCFS; // default

void changePCBState(PCB* process, PROCESS_STATE newState, int timeOfTransition)
{
	OFMLogState(timeOfTransition, process, newState);
	printf("Transition at %d ms.\tProcess %d changed state from %s to %s\n",
		timeOfTransition, process->pid, stateToStr(process->state), stateToStr(newState));
	process->state = newState;
}


void SimulatorSetProcesses(PCB** inputProcesses, int nb)
{
	processes = inputProcesses;
	nbProcesses =  nb;
}

void SimulatorSetMode(SCHELDULER_MODE MODESET)
{
	MODE = MODESET;
}

void SimulatorSetAlgorithm(SCHELDULER_ALGORITHM ALGO)
{
	ALGORITHM = ALGO;
}

// increments the timer and update the process counters
void step(int* time)
{
	// check every PCB and update their values
	for (int i = 0; i < nbProcesses; ++i)
	{
		PCB* p = processes[i];
		switch(p->state)
		{
			case RUNNING:
				p->totalCPUTimeUsed++;
				p->remainingBeforeIORequestTime -= p->remainingBeforeIORequestTime < 0?0:1;
				break;
			case WAITING:
				p->remainingIODuration--;
				break;
			case READY:
				p->waitTime++;
				break;
			default:
				break;
		}
	}
	(*time)++;
}

int endOfSimulation(int* time)
{
	// all the process are not or the time limit has not been reached
	return ((PCBQSize(terminatedQueue) == nbProcesses) || (*time >= SIMULATION_TIME_LIMIT));
}

PCB* pickNextProcess()
{
	switch(ALGORITHM)
	{
		case ALGORITHM_PRIORITY:
			return PCBHPop(readyHeap);
		case ALGORITHM_FCFS:
		default:
			return PCBQPop(readyQueue);
	}
}

// queues/heap initialisation
void initStructures()
{
	newQueue = PCBQCreate(nbProcesses);
	switch(ALGORITHM)
	{
		case ALGORITHM_PRIORITY:
			readyHeap = PCBHCreate(nbProcesses);
		case ALGORITHM_FCFS:
		default:
			readyQueue = PCBQCreate(nbProcesses);
	}
	terminatedQueue = PCBQCreate(nbProcesses);
}

// clean and delete all the data structures
void deleteStructures()
{
	PCBQDelete(newQueue);
	switch(ALGORITHM)
	{
		case ALGORITHM_PRIORITY:
			PCBHDelete(readyHeap);
		case ALGORITHM_FCFS:
		default:
			PCBQDelete(readyQueue);
	}
	PCBQDelete(terminatedQueue);
}

// insert a process in the ready queue/heap
void insertREADYProcess(PCB* p)
{
	switch(ALGORITHM)
	{
		case ALGORITHM_PRIORITY:
			PCBHInsert(readyHeap, p);
		case ALGORITHM_FCFS:
		default:
			PCBQInsert(readyQueue, p);
	}
}

// return 1 if there are no READY process waiting, else 0
int noReadyProcess()
{
	switch(ALGORITHM)
	{
		case ALGORITHM_PRIORITY:
			return PCBHisEmpty(readyHeap);
		case ALGORITHM_FCFS:
		default:
			return PCBQisEmpty(readyQueue);
	}
}

void SimulatorRun()
{
	initStructures();
	int inputProcessIndex = 0;
	int time = 0; // in ms
	PCB* runningProcess = NULL;

	printf("Starting Simulation of %d process%s in %s mode with %s algorithm...\n",nbProcesses,(nbProcesses>1?"es":""), modeToStr(MODE), algorithmToStr(ALGORITHM));
	// simulation loop
	while(!endOfSimulation(&time)) // while the simulation is not done
	{
		//is there a new process that should be created ?
		if(inputProcessIndex < nbProcesses && processes[inputProcessIndex]->arrivalTime == time)
		{
			// state change to NEW, add it to newQueue
			/** UNDEFINED -> NEW **/
			changePCBState(processes[inputProcessIndex], NEW, time);
			PCBQInsert(newQueue, processes[inputProcessIndex]);
			inputProcessIndex++;
			continue;
		}


		// if some process finished managing their IO
		for (int i = 0; i < nbProcesses; ++i)
		{
			PCB* p = processes[i];
			if(p->state == WAITING && p->remainingIODuration == 0)
			{
				/** WAITING -> READY **/
				changePCBState(p, READY, time);
				p->remainingBeforeIORequestTime = p->IOFrequency;
				insertREADYProcess(p);
			}
		}
		

		// If there is NEW process admit them
		if(!PCBQisEmpty(newQueue))
		{
			// state go to READY, add it to readyQueue, remove from newQueue
			PCB* p = PCBQPop(newQueue);
			/** NEW -> READY **/
			changePCBState(p, READY, time);
			insertREADYProcess(p);
			continue;
		}

		// if nobody is RUNNING run the next one ready
		if(runningProcess == NULL)
		{
			if(!noReadyProcess())
			{
				// we take the first one. FCFS
				PCB* p = pickNextProcess(); // SCHELDULER JOB
				/** READY -> RUNNING **/
				changePCBState(p, RUNNING, time);
				runningProcess = p;
				continue;
			}
		}
		else
		{
			// there is a running process
			// if the running process is done
			if(runningProcess->totalCPUTime == runningProcess->totalCPUTimeUsed)
			{
				// state change to TERMINATED, add it to terminatedQueue
				/** RUNNING -> TERMINATED **/
				changePCBState(runningProcess, TERMINATED, time);
				PCBQInsert(terminatedQueue, runningProcess);
				runningProcess = NULL;
				continue;
			}
			// If an IO request occurs on the running process
			else if(runningProcess->remainingBeforeIORequestTime == 0 && runningProcess->totalCPUTimeUsed > 0)
			{
				// change to WAITING STATE, free the RUNNING state
				/** RUNNING -> WAITING **/
				changePCBState(runningProcess, WAITING, time);
				runningProcess->remainingBeforeIORequestTime = runningProcess->IOFrequency; // reset IORequest timer
				runningProcess->remainingIODuration = runningProcess->IODuration;
				runningProcess = NULL;
				continue;
			}
		}
		
		// if we reach this point, there was nothing more to do at this instant. We go to the next instant
		step(&time);
	}

	if(time >= SIMULATION_TIME_LIMIT) printf("SIMULATION HAS REACHED TIME LIMIT OF %d ms.\n", SIMULATION_TIME_LIMIT);
	printf("End of Simulation at %d ms.\n", time);

	deleteStructures();
}

void SimulatorReset()
{
	for (int i = 0; i < nbProcesses; ++i)
	{
		PCBReset(processes[i]);
	}
}