CC=gcc
MAKE=make

# Get the OS type (a la https://stackoverflow.com/a/52062069/7171141)
ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

# Get the correct file seperator
ifeq ($(detected_OS),Windows)
# Two slashes so % isn't escaped. When there isn't a following %,
# Windows is OK with two slashes in names
	SL=\\
	DEL=del /S /Q
else
	SL=/
	DEL=rm -rf
endif

OBJLIST=main.o error.o lex.o re.o rd_parse.o TAC.o assembly.o
OUTLIST=JCOMP.exe

# Release/debug split from https://stackoverflow.com/a/34574113/7171141
ifeq ($(BUILD),debug)
	FLAGS=--std=c11 -Og -Wall -Wextra -Wpedantic -Wno-switch -DDEBUG -D$(TARGET)
	PRE=$(SL)$(TARGET)$(SL)Debug$(SL)
	OUT=$(addprefix bin$(PRE),$(OUTLIST))
	OBJ=$(addprefix obj$(PRE),$(OBJLIST))
else
	FLAGS=--std=c11 -O3 -D$(TARGET)
	PRE=$(SL)$(TARGET)$(SL)Release$(SL)
	OUT=$(addprefix bin$(PRE),$(OUTLIST))
	OBJ=$(addprefix obj$(PRE),$(OBJLIST))
endif

all:
	$(MAKE) M68000
	$(MAKE) M68000-debug

M68000:
	$(MAKE) release TARGET=M68000
M68000-debug:
	$(MAKE) debug   TARGET=M68000

release debug:
	$(MAKE) _output BUILD=$@ TARGET=$(TARGET)

$(OBJ) : obj$(PRE)%.o : src$(SL)%.c
	$(CC) $(FLAGS) -c $< -o $@

_output: $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(OUT)

clean:
	$(DEL) obj bin
	
