#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Simulator.h"
#include "OutputFileManager.h"
#include "PCB.h"
#include "PCBQueue.h"
#include "PCBHeap.h"


static PCB** processes = NULL;
static int nbProcesses = 0;
static PCBQueue* newQueue = NULL;	// a queue containing the processes in NEW state
static PCBQueue* readyQueue = NULL;	// a queue containing the processes in READY state. Used for ALGORITHM_FCFS
static PCBHeap* readyHeap = NULL;	// a heap containing the processes in READY state. Used for ALGORITHM_PRIORITY
static PCBQueue* terminatedQueue = NULL;	// a queue containing the processes in TERMINATED state
static SCHELDULER_MODE MODE = MODE_NONPREEMPTIVE; // default
static SCHELDULER_ALGORITHM ALGORITHM = ALGORITHM_FCFS; // default
static VERBOSE_LEVEL VERBOSE = TRACE; // default
static struct SimulatorStatistic
{
	double throughput;
	double turnaround;
	double waitTime;
	double responseTime;
	double CPUUtilization;
} globalStatistics;


char* modeToStr(SCHELDULER_MODE mode)
{
	return (mode==MODE_PREEMPTIVE?"PREEMPTIVE":
		(mode==MODE_NONPREEMPTIVE?"NONPREEMPTIVE":"UNKNOWN"
	));
}

char* algorithmToStr(SCHELDULER_ALGORITHM algorithm)
{
	return (algorithm==ALGORITHM_FCFS?"FCFS":
		(algorithm==ALGORITHM_PRIORITY?"PRIORITY":
			(algorithm==ALGORITHM_SJF?"SJF":"UNKNOWN"
	)));
}

void changePCBState(PCB* process, PROCESS_STATE newState, int timeOfTransition)
{
	OFMLogState(timeOfTransition, process, newState);
	if(VERBOSE == TRACE) printf("Transition at %d ms.\tProcess %d changed state from %s to %s\n",
		timeOfTransition, process->pid, stateToStr(process->state), stateToStr(newState));
	process->state = newState;
}

void resetGlobalStats()
{
	globalStatistics.throughput = 0;
	globalStatistics.turnaround = 0;
	globalStatistics.waitTime = 0;
	globalStatistics.responseTime = 0;
	globalStatistics.CPUUtilization = 0;
}

void SimulatorSetProcesses(PCB** inputProcesses, int nb)
{
	processes = inputProcesses;
	nbProcesses =  nb;
	// Sort PCBs by Arrival Time
	qsort(processes, nbProcesses, sizeof(PCB*), PCBCompareArrivalTime);
	resetGlobalStats();
}

void SimulatorSetMode(SCHELDULER_MODE MODESET)
{
	MODE = MODESET;
}

void SimulatorSetAlgorithm(SCHELDULER_ALGORITHM ALGO)
{
	ALGORITHM = ALGO;
}

void SimulatorSetVerbose(VERBOSE_LEVEL level)
{
	VERBOSE = level;
}

// increments the timer and update the process counters, and the statistics
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

				globalStatistics.turnaround++;
				globalStatistics.CPUUtilization++;
				break;
			case WAITING:
				p->remainingIODuration--;

				globalStatistics.turnaround++;
				break;
			case READY:
				p->waitTime++;

				globalStatistics.turnaround++;
				globalStatistics.waitTime++;
				break;
			default:
				break;
		}
	}
	(*time)++;
}

PCB* pickNextProcess()
{
	switch(ALGORITHM)
	{
		case ALGORITHM_PRIORITY:
		case ALGORITHM_SJF:
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
			readyHeap = PCBHCreate(nbProcesses, &PCBComparePriority); // heap based on priority
		case ALGORITHM_SJF:
			readyHeap = PCBHCreate(nbProcesses, &PCBCompareTotalCPUTime); // heap based on TotalCPUTime
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
		case ALGORITHM_SJF:
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
		case ALGORITHM_SJF:
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
		case ALGORITHM_SJF:
			return PCBHisEmpty(readyHeap);
		case ALGORITHM_FCFS:
		default:
			return PCBQisEmpty(readyQueue);
	}
}

int endOfSimulation(int* time)
{
	// all the process are not or the time limit has not been reached
	return ((PCBQSize(terminatedQueue) == nbProcesses) || (*time >= SIMULATION_TIME_LIMIT));
}

void SimulatorRun()
{
	initStructures();
	int inputProcessIndex = 0;
	int time = 0; // in ms
	PCB* runningProcess = NULL;

	printf("Starting Simulation of %d process%s in %s mode with %s algorithm...\n",nbProcesses,(nbProcesses>1?"es":""), modeToStr(MODE), algorithmToStr(ALGORITHM));
	// simulation loop  PROCESS LIFECYCLE IMPLEMENTATION
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
				p->remainingBeforeIORequestTime = p->IOFrequency; // reset IORequest timer
				insertREADYProcess(p);
			}
		}
		

		// If there is NEW process admit them, long-term schelduler job
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
				if(runningProcess->IOFrequency == 0)// There were no output request dureing the process
				{
					// Response Time moment (program finished without IO)
					globalStatistics.responseTime += time - runningProcess->arrivalTime;
				}
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
				if(runningProcess->totalCPUTimeUsed == runningProcess->IOFrequency)// first time the program request for IO
				{
					// Response Time moment (first IO)
					globalStatistics.responseTime += time - runningProcess->arrivalTime;
				}
				// change to WAITING STATE, free the RUNNING state
				/** RUNNING -> WAITING **/
				changePCBState(runningProcess, WAITING, time);
				runningProcess->remainingIODuration = runningProcess->IODuration;
				runningProcess = NULL;
				continue;
			}
		}
		

		// if we reach this point, there was nothing more to do at this instant. We go to the next instant
		if(VERBOSE == GANT){
			if(time > 0) printf("| ");
			if(runningProcess == NULL) printf("    ");
			else printf("P%2d ",runningProcess->pid);
		}
		step(&time);
	}
	if(VERBOSE == GANT) printf("\n");

	if(time >= SIMULATION_TIME_LIMIT) printf("SIMULATION HAS REACHED TIME LIMIT OF %d ms.\n", SIMULATION_TIME_LIMIT);
	printf("End of Simulation at %d ms.\n", time);

	// update / compute the globalStatistics
	globalStatistics.throughput = (double)nbProcesses / time;
	globalStatistics.turnaround /= (double)nbProcesses;
	globalStatistics.waitTime /= (double)nbProcesses;
	globalStatistics.CPUUtilization /= (double)time;
	globalStatistics.responseTime /= (double)nbProcesses;
	deleteStructures();
}

void SimulatorPrintStats()
{
	printf("Statistics of the system : \n");
	printf("Throughput : %.2f process/ms\n", globalStatistics.throughput);
	printf("Average Turnaround Time : %.2f ms\n", globalStatistics.turnaround);
	printf("Average Waiting Time : %.2f ms\n", globalStatistics.waitTime);
	printf("Average Response Time : %.2f ms\n", globalStatistics.responseTime);
	printf("CPU Utilization : %.2f %%\n", globalStatistics.CPUUtilization*100);
}

void SimulatorReset()
{
	for (int i = 0; i < nbProcesses; ++i)
	{
		PCBReset(processes[i]);
	}
	resetGlobalStats();
}