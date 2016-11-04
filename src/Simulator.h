#if ! defined ( SIMULATOR_H )
#define SIMULATOR_H

#include "PCB.h"

#define SIMULATION_TIME_LIMIT 1000000 // 1000000 miliseconds, to prevent infinite loop, adapt if needed

// enum for the different schelduler mode
typedef enum SCHELDULER_MODE
{
	MODE_PREEMPTIVE, MODE_NONPREEMPTIVE		// PREEMPTIVE Mode is not implemented yet
} SCHELDULER_MODE;

// enum for the different schelduler algorithms
typedef enum SCHELDULER_ALGORITHM
{
	ALGORITHM_FCFS, ALGORITHM_PRIORITY, ALGORITHM_SJF
} SCHELDULER_ALGORITHM;

// enum for the different verbose level (output)
typedef enum VERBOSE_LEVEL
{
	NO_OUTPUT, TRACE, GANT
} VERBOSE_LEVEL;

char* modeToStr(SCHELDULER_MODE mode);

char* algorithmToStr(SCHELDULER_ALGORITHM algorithm);


void SimulatorSetProcesses(PCB** inputProcesses, int nb);

void SimulatorSetMode(SCHELDULER_MODE MODES);

void SimulatorSetAlgorithm(SCHELDULER_ALGORITHM ALGO);

void SimulatorSetVerbose(VERBOSE_LEVEL level);

void SimulatorRun();

void SimulatorPrintStats();

void SimulatorReset();

#endif // SIMULATOR_H