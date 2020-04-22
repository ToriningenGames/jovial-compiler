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
                error("No input");
        };
        FILE *infile = fopen(argv[1], "rt");
        mainbuf[fread(mainbuf, 1, 32767, infile)] = '\0';
        fclose(infile);
        newInput(mainbuf);
        struct lex_token tokList[32768];
        tokList[0] = (struct lex_token){0xFF, 0xFF};
        for (int i = 1; tokList[i-1].kind; i++) {
                tokList[i] = getToken();
        }
        parseInit(tokList+1);
        printTree();
        parseAll();
        printTree();
        error("Critical failure");
}
