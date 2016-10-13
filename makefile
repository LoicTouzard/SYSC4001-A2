EXE=Simulator
INT=

INTERFACEPATH=src/
SOURCEPATH=src/
EXEPATH=./


SRC:=$(addprefix $(SOURCEPATH),$(INT:.h=.c))
OBJ:=$(addprefix $(SOURCEPATH),$(INT:.h=.o))
INT:=$(addprefix $(SOURCEPATH),$(INT))

MainSRC=main.c
MainOBJ=$(MainSRC:.c=.o)


ECHO=@echo

CLEAN=clean
RM=rm
RMFlags=-f

Compilator=@gcc
CompilationsOptions=-Wall -pedantic
Linker=@gcc
LinkerOptions=

all: $(EXE)

$(EXE): $(SOURCEPATH)$(MainOBJ) $(OBJ)
	$(ECHO) "Link Edition for $(EXE)"
	$(Linker) -o $(EXEPATH)$(EXE) $(SOURCEPATH)$(MainOBJ) $(LinkerOptions)
	
$(SOURCEPATH)$(MainOBJ): $(SOURCEPATH)$(MainSRC) $(INT)
	$(ECHO) "Compilation of $(MainSRC)"
	$(Compilator) -o $(SOURCEPATH)$(MainOBJ) -c $(SOURCEPATH)$(MainSRC) $(CompilationsOptions)

%.o: %.c
	$(ECHO) "Compilation of $<"
	$(Compilator) -c $< $(CompilationsOptions)
	
# ***** CLEAN ***** #

.PHONY: $(CLEAN)
$(CLEAN):
	$(RM) $(RMFlags) $(EXEPATH)$(EXE) $(OBJ) $(SOURCEPATH)$(MainOBJ)
