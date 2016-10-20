#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InputFileManager.h"
#include "PCB.h"

#define SIMULATION_TIME_LIMIT 1000000 // 1000 seconds, to prevent infinite loop
#define PREEMPTIVE 1
#define NONPREEMPTIVE 2


void printAllProcesses(PCB** processes, int nbProcesses)
{
	for (int i = 0; i < nbProcesses; ++i) PCBPrint(processes[i]);
}

void changePCBState(PCB* process, PROCESS_STATE newState, int timeOfTransition)
{
	printf("Transition at %d ms.\tProcess %d changed state from %s to %s\n",
		timeOfTransition, process->pid, stateToStr(process->state), stateToStr(newState));
	process->state = newState;
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Use : ./Simulator <input_file> <output_file>\n");
		return EXIT_FAILURE;
	}

	if(IFMOpen(argv[1]) == -1) return EXIT_FAILURE;

	PCB** processes = NULL;
	int nbProcesses = IFMReadProcesses(&processes);
	printf("%d Process%s created from input\n", nbProcesses, (nbProcesses>1?"es":""));

	// Sort PCBs by Arrival Time
	qsort(processes, nbProcesses, sizeof(PCB*), PCBCmp);

	printAllProcesses(processes, nbProcesses);


	// execution goes here
	// Init simulation
	PCBQueue* newQueue = PCBQCreate(nbProcesses);
	PCBQueue* readyQueue = PCBQCreate(nbProcesses);
	PCBQueue* terminatedQueue = PCBQCreate(nbProcesses);
	int inputProcessIndex = 0;
	int time = 0; // in ms
	PCB* runningProcess = NULL;
	const int MODE = NONPREEMPTIVE;


	// simulation loop
	printf("Starting Simulation...\n");
	while((PCBQSize(terminatedQueue) != nbProcesses) && (time < SIMULATION_TIME_LIMIT)) // while all the process are not done
	{
		//FCFS Schelduler no preemption

		//is there a new process that should be created ?
		if(inputProcessIndex < nbProcesses && processes[inputProcessIndex]->arrivalTime == time)
		{
			// state change to NEW, add it to newQueue
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
				changePCBState(runningProcess, TERMINATED, time);
				PCBQInsert(terminatedQueue, runningProcess);
				runningProcess = NULL;
				continue;
			}
			// If an IO request occurs on the running process
			else if(runningProcess->remainingBeforeIORequestTime == 0 && runningProcess->totalCPUTimeUsed > 0)
			{
				// change to WAITING STATE, free the RUNNING state
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

	for (int i = 0; i < nbProcesses; ++i)
	{
		PCBDelete(processes[i]);
	}
	free(processes);


	if(IFMClose() == -1) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}