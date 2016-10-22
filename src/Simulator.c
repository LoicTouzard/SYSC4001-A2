#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Simulator.h"
#include "PCB.h"


static PCB** processes = NULL;
static int nbProcesses = 0;
static PCBQueue* newQueue = NULL;
static PCBQueue* readyQueue = NULL;
static PCBQueue* terminatedQueue = NULL;
static SCHELDULER_MODE MODE;

void changePCBState(PCB* process, PROCESS_STATE newState, int timeOfTransition)
{
	printf("Transition at %d ms.\tProcess %d changed state from %s to %s\n",
		timeOfTransition, process->pid, stateToStr(process->state), stateToStr(newState));
	process->state = newState;
}


void SimulatorInit(PCB** inputProcesses, int nb, SCHELDULER_MODE MODESET)
{
	// Init simulation
	// queues initialisation
	if(newQueue != NULL) PCBQDelete(newQueue);
	newQueue = PCBQCreate(nb);
	if(readyQueue != NULL) PCBQDelete(readyQueue);
	readyQueue = PCBQCreate(nb);
	if(terminatedQueue != NULL) PCBQDelete(terminatedQueue);
	terminatedQueue = PCBQCreate(nb);

	processes = inputProcesses;
	MODE = MODESET;
	nbProcesses =  nb;
}

void SimulatorRun()
{
	int inputProcessIndex = 0;
	int time = 0; // in ms
	PCB* runningProcess = NULL;


	// simulation loop
	printf("Starting Simulation...\n");
	while((PCBQSize(terminatedQueue) != nbProcesses) && (time < SIMULATION_TIME_LIMIT)) // while all the process are not done
	{
		//FCFS Schelduler no preemption

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
				PCBQInsert(readyQueue, p);
			}
		}
		

		// If there is NEW process admit them
		if(!PCBQisEmpty(newQueue))
		{
			// state go to READY, add it to readyQueue, remove from newQueue
			PCB* p = PCBQPop(newQueue);
			/** NEW -> READY **/
			changePCBState(p, READY, time);
			PCBQInsert(readyQueue, p);
			continue;
		}

		// if nobody is RUNNING run the next one ready
		if(runningProcess == NULL)
		{
			if(!PCBQisEmpty(readyQueue))
			{
				// we take the first one. FCFS
				PCB* p = PCBQPop(readyQueue);
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
		time++;
	}




	if(time >= SIMULATION_TIME_LIMIT) printf("SIMULATION HAS REACHED TIME LIMIT OF %d ms.\n", SIMULATION_TIME_LIMIT);
	printf("End of Simulation at %d ms.\n", time);


	PCBQDelete(newQueue);
	PCBQDelete(readyQueue);
	PCBQDelete(terminatedQueue);
}