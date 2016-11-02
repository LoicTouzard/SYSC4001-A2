# SYSC4001-A2

Course SYSC4001 - Assignment 2 - Carleton University 


### Input file

Input file is input.txt. Each line describe a process and its caracteristics.
Structure is the following :
`pid;ArrivalTime;TotalCPUTime;IOFrequency;IODuration`

`pid` is an integer  
`ArrivalTime` is a positive or null integer  
`TotalCPUTime` is a positive or null integer  
`IOFrequency` is a positive integer. If set to 0 there is no IO  
`IODuration` is a positive or null integer  


### Makefile

In root directory.  
To build the project use the command `make`  
To clean the project use the command `make clean`  
To run the project use the command `./Simulator`  
To build and run (or just run if it is already built) use the command `make run`  
