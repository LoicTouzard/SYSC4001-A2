#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "a.h"

int main(int argc, char* argv[])
{
	int i = 0;
	for(i=0; i<argc; i++)
	{
		printf("%s\n", argv[i]);
	}
	return 0;
}