#include <stdio.h>
#include <stdarg.h>
#include "inc/defines.h"

const char *lexNames[] = {
        "LEX_NONE", "LEX_NUMLIT", "LEX_STRLIT", "LEX_DBLSTR", "LEX_OP", "LEX_IDEN", "LEX_SEP"
};


//Dummy function, to be placed and written later
void newOutput(char *name)
{
        ;
}


static int screamlevel = 0;
void debugOut(int prio, char *format, ...)
{
        if (prio < screamlevel) {
                va_list arg;
                va_start(arg, format);
                vprintf(format, arg);
                va_end(arg);
        }
}

int main(int argc, char **argv)
{
        screamlevel = 999;
        char mainbuf[32768];
        if (argc == 1) {
                puts("Program error: No input");
        };
        FILE *infile = fopen(argv[1], "rt");
        mainbuf[fread(mainbuf, 1, 32767, infile)] = '\0';
        fclose(infile);
        lexInit();
        newInput(mainbuf);
        getProgram();
        puts("Program error: Critical failure; unfinished");
}
