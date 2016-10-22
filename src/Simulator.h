#if ! defined ( SIMULATOR_H )
#define SIMULATOR_H

#include "PCB.h"

#define SIMULATION_TIME_LIMIT 1000000 // 1000 seconds, to prevent infinite loop

// enum for the different schelduler mode
typedef enum SCHELDULER_MODE
{
	MODE_PREEMPTIVE, MODE_NONPREEMPTIVE
} SCHELDULER_MODE;


void SimulatorInit(PCB** processes, int nbProcesses, SCHELDULER_MODE MODE);

void SimulatorRun();

#endif // SIMULATOR_H