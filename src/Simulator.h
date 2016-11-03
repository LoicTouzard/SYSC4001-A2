#if ! defined ( SIMULATOR_H )
#define SIMULATOR_H

#include "PCB.h"

#define SIMULATION_TIME_LIMIT 1000 // 1000 seconds, to prevent infinite loop, adapt if needed

// enum for the different schelduler mode
typedef enum SCHELDULER_MODE
{
	MODE_PREEMPTIVE, MODE_NONPREEMPTIVE
} SCHELDULER_MODE;

typedef enum SCHELDULER_ALGORITHM
{
	ALGORITHM_FCFS, ALGORITHM_PRIORITY, ALGORITHM_SJF
} SCHELDULER_ALGORITHM;

char* modeToStr(SCHELDULER_MODE mode);

char* algorithmToStr(SCHELDULER_ALGORITHM algorithm);


void SimulatorSetProcesses(PCB** inputProcesses, int nb);

void SimulatorSetMode(SCHELDULER_MODE MODES);

void SimulatorSetAlgorithm(SCHELDULER_ALGORITHM ALGO);

void SimulatorSetVerbose(int level);

void SimulatorRun();

void SimulatorReset();

#endif // SIMULATOR_H