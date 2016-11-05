# SYSC4001-A2
Kariharan Thilagakumar 100922048
Loic Touzard 101057279

Course SYSC4001 - Assignment 2 - Carleton University 

This is a Task lifecycle simulator with different schelduler algorithm.

### Input file

Input files are in `data/` directory as well as output files. Each line describe a process and its caracteristics.
Structure is the following :
`pid;ArrivalTime;TotalCPUTime;IOFrequency;IODuration;Priority`  

`pid` is an integer  
`ArrivalTime` is a positive or null integer  
`TotalCPUTime` is a positive or null integer  
`IOFrequency` is a positive integer. If set to 0 there is no IO  
`IODuration` is a positive or null integer  
`Priority` is optional, it is a positive or null integer, 0 is the lowest priority  

You can use comments with line beginning with `#`  


### Makefile

In root directory.  
To build the project use the command `make`  
To clean the project use the command `make clean`  
To build and run (or just run if it is already built) use the command `make run` it will use the default files `data/input.txt` and `data/output.txt`  FCFS algorithm and TRACE output.  

### Run the Programm

There is one program `Simulator` which can run many different algorithms.  
Use : `./Simulator <input_file> <output_file> <ALGORITHM> <VERBOSE>`  
* `<output_file>` is the name of the output file, the file will be created or override a previous one.
* `<input_file>`  is the name of the input file (you may use the ones in `data/` directory)
* `<ALGORITHM>`   can be `FCFS`, `PRIORITY`, `SJF` to run the differents algorithm.
* `<VERBOSE>`     (optional) can be `TRACE`, `GANTT`. Default gives none.  

`TRACE` give a log at each event (changing state) on the screen.  
`GANTT` display a gantt chart of the simulation.  

There are statistics displayed at the end of the simulation.

### Run the tests

Use the following commands :  

* `make runpart1` will run with FCFS algorithm the processes in the files `data/inputPart1c.txt` and `data/inputPart1d.txt` as described in the assignment.  
* `make runtests` will run all the test described and commented in the `Part2_Report.pdf` file. All the ourput files goes to `dtaa/` directory as `output<CaseName><ALGORITHM>.txt`  

You can also run `tests.sh` that just run the command above...
