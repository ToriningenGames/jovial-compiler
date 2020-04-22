#include <stdio.h>
#include <stdarg.h>
#include "inc/defines.h"

char *lexNames[] = {
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
        struct lex_token a;
        lexInit();
        while ((a = getToken()).kind) {
                printf("Type: %s\tID: %li",
                    lexNames[a.kind],
                    a.id);
                switch (a.kind) {
                        case LEX_NUMLIT :
                        case LEX_DBLSTR :
                        case LEX_IDEN :
                                printf("\t");
                        case LEX_STRLIT :
                                printf("\tValue: %s\n", stringTable[a.id]);
                                break;
                        case LEX_SEP :
                        case LEX_OP :
                                printf("\tValue: %c", (char)a.id);
                                if (a.id > 256) {
                                        printf("%c", (char)(a.id>>8));
                                };
                                puts("");
                                break;
                        default :
                                puts("");
                }
        }
        error("Critical failure");
}
