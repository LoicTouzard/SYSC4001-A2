#if ! defined ( INPUTFILEMANAGER_H )
#define INPUTFILEMANAGER_H

#include "PCB.h"

int openInputFile(const char* filename);

int closeInputFile();

int readProcesses(PCB*** processes);
#endif // INPUTFILEMANAGER_H