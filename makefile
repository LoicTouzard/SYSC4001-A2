EXE=Simulator
INT=InputFileManager.h PCB.h Simulator.h OutputFileManager.h PCBQueue.h PCBHeap.h

INTERFACEPATH=src/
SOURCEPATH=src/
DATAPATH=data/
EXEPATH=./


SRC:=$(addprefix $(SOURCEPATH),$(INT:.h=.c))
OBJ:=$(addprefix $(SOURCEPATH),$(INT:.h=.o))
INT:=$(addprefix $(INTERFACEPATH),$(INT))

MainSRC=main.c
MainOBJ=$(MainSRC:.c=.o)


ECHO=@echo

CLEAN=clean
RUN=run
RUNTESTS=runtests
RUNPART1=runpart1
RM=rm
RMFlags=-f

Compilator=@gcc
CompilationsOptions=-Wall -pedantic -std=c11
Linker=@gcc
LinkerOptions=

all: $(EXE)

# ***** COMPILE ***** #

$(EXE): $(SOURCEPATH)$(MainOBJ) $(OBJ)
	$(ECHO) "Link Edition for $(EXE)"
	$(Linker) -o $(EXEPATH)$(EXE) $(SOURCEPATH)$(MainOBJ) $(OBJ) $(LinkerOptions)
	
$(SOURCEPATH)$(MainOBJ): $(SOURCEPATH)$(MainSRC) $(INT)
	$(ECHO) "Compilation of $(SOURCEPATH)$(MainSRC)"
	$(Compilator) -o $(SOURCEPATH)$(MainOBJ) -c $(SOURCEPATH)$(MainSRC) $(CompilationsOptions)

%.o: %.c
	$(ECHO) "Compilation of $<"
	$(Compilator) -c $< -o $@ $(CompilationsOptions) 


# ***** RUN ***** #
$(RUN): $(EXE)
	$(ECHO) "Program execution"
	$(EXEPATH)$(EXE) $(DATAPATH)input.txt $(DATAPATH)output.txt FCFS TRACE

$(RUNPART1): $(EXE)
	$(ECHO) "Running Part 1 test"
	$(ECHO) "Part 1 c) FCFS without IO"
	$(EXEPATH)$(EXE) $(DATAPATH)input1c.txt $(DATAPATH)output1c.txt FCFS GANTT
	$(ECHO)
	$(ECHO) "Part 1 d) FCFS with IO"
	$(EXEPATH)$(EXE) $(DATAPATH)input1d.txt $(DATAPATH)output1d.txt FCFS GANTT

# ***** TEST ***** #
$(RUNTESTS): $(EXE)
	$(ECHO) "Running all the tests"
	$(ECHO) "Convoy effect tests"
	$(EXEPATH)$(EXE) $(DATAPATH)inputConvoy.txt $(DATAPATH)outputConvoyFCFS.txt FCFS GANTT
	$(EXEPATH)$(EXE) $(DATAPATH)inputConvoy.txt $(DATAPATH)outputConvoySJF.txt SJF GANTT
	$(ECHO)
	$(ECHO) "Different durations process tests"
	$(EXEPATH)$(EXE) $(DATAPATH)inputManyDurations.txt $(DATAPATH)outputManyDurationsFCFS.txt FCFS GANTT
	$(EXEPATH)$(EXE) $(DATAPATH)inputManyDurations.txt $(DATAPATH)outputManyDurationsSJF.txt SJF GANTT
	$(ECHO)
	$(ECHO) "Different short and long with delays tests"
	$(EXEPATH)$(EXE) $(DATAPATH)inputHappylyDelayed.txt $(DATAPATH)outputHappylyDelayedSJF.txt SJF GANTT
	$(EXEPATH)$(EXE) $(DATAPATH)inputSadlyDelayed.txt $(DATAPATH)outputSadlyDelayedSJF.txt SJF GANTT
	$(ECHO)
	$(ECHO) "Many user processes and one high priority process tests"
	$(EXEPATH)$(EXE) $(DATAPATH)inputPriority.txt $(DATAPATH)outputPriorityFCFS.txt FCFS GANTT
	$(EXEPATH)$(EXE) $(DATAPATH)inputPriority.txt $(DATAPATH)outputPrioritySJF.txt SJF GANTT
	$(EXEPATH)$(EXE) $(DATAPATH)inputPriority.txt $(DATAPATH)outputPriorityPRIORITY.txt PRIORITY GANTT

# ***** CLEAN ***** #

.PHONY: $(CLEAN)
$(CLEAN):
	$(ECHO) "Cleaning the workspace..."
	$(RM) $(RMFlags) $(EXEPATH)$(EXE) $(OBJ) $(SOURCEPATH)$(MainOBJ)
