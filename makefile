EXE=Simulator
INT=InputFileManager.h PCB.h

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
run: $(EXE)
	$(ECHO) "Program execution"
	$(EXEPATH)$(EXE) $(DATAPATH)input.txt $(DATAPATH)output.txt

# ***** CLEAN ***** #

.PHONY: $(CLEAN)
$(CLEAN):
	$(ECHO) "Cleaning the workspace..."
	$(RM) $(RMFlags) $(EXEPATH)$(EXE) $(OBJ) $(SOURCEPATH)$(MainOBJ)
