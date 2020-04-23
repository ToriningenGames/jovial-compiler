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

OBJLIST=main.o error.o lex.o re.o parse.o
OUTLIST=JCOMP.exe

# Release/debug split from https://stackoverflow.com/a/34574113/7171141
ifeq ($(BUILD),release)
	FLAGS=--std=c11 -O3
	PRE=$(SL)Release$(SL)
	OUT=$(addprefix bin$(PRE),$(OUTLIST))
	OBJ=$(addprefix obj$(PRE),$(OBJLIST))
else
	FLAGS=--std=c11 -Og -Wall -Wextra -Wpedantic -Wno-switch -DDEBUG
	PRE=$(SL)Debug$(SL)
	OUT=$(addprefix bin$(PRE),$(OUTLIST))
	OBJ=$(addprefix obj$(PRE),$(OBJLIST))
endif

all:
	$(MAKE) release
	$(MAKE) debug

release debug:
	$(MAKE) _output BUILD=$@

$(OBJ) : obj$(PRE)%.o : src$(SL)%.c
	$(CC) $(FLAGS) -c $< -o $@

_output: $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(OUT)

clean:
	$(DEL) obj bin
	
