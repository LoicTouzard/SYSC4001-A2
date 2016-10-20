#if ! defined ( INPUTFILEMANAGER_H )
#define INPUTFILEMANAGER_H

#include "PCB.h"

int IFMOpen(const char* filename);

int IFMClose();

int IFMReadProcesses(PCB*** processes);
#endif // INPUTFILEMANAGER_H