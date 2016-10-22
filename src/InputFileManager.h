#if ! defined ( INPUTFILEMANAGER_H )
#define INPUTFILEMANAGER_H

/* Manager for the input file */

#include "PCB.h"

/*
	Open the input file
	Params : 
		- filename : name of the input file
	Return value :
		- -1 if failed
		- 0 if successed

 */
int IFMOpen(const char* filename);

/*
	Close the input file
	Return value :
		- -1 if failed
		- 0 if successed
 */
int IFMClose();

/* 
	Parse all the input file create dynamically a PCB for each.
	All the PCB filed in a PCB* array (PCB**) given as parameter by adress
	If the file has a misformatted line, it will skip it.
	Params :
		- processes is an array of PCB* given by address to change its size (so PCB***)
	Return value :
		- int number of PCB read
 */
int IFMReadProcesses(PCB*** processes);

#endif // INPUTFILEMANAGER_H