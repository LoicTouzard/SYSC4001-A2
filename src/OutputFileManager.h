#if ! defined ( OUTPUTFILEMANAGER_H )
#define OUTPUTFILEMANAGER_H

/* Manager for the output file */

#include "PCB.h"

/*
	Open the Output file
	Params : 
		- filename : name of the input file
	Return value :
		- -1 if failed
		- 0 if successed

 */
int OFMOpen(const char* filename);

/*
	Close the output file
	Return value :
		- -1 if failed
		- 0 if successed
 */
int OFMClose();

/*
	Log a state change into the output file
	return
 */
int OFMLogState(int time, PCB* process, PROCESS_STATE newState);


#endif // OUTPUTFILEMANAGER_H