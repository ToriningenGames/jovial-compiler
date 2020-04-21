#include <stdio.h>
#include "inc/defines.h"

char *lexNames[] = {
        "LEX_NONE", "LEX_NUMLIT", "LEX_STRLIT", "LEX_DBLSTR", "LEX_OP", "LEX_IDEN", "LEX_SEP"
};


//Dummy function, to be placed and written later
void newOutput(char *name)
{
        ;
}


int main(int argc, char **argv)
{
        //Windows-specific using standard input as file
        char mainbuf[32768];
        fread(mainbuf, 1, 32768, stdin);
        newInput(mainbuf);
        newOutput("CON");
        struct lex_token a;
        lexInit();
        while ((a = getToken()).kind) {
                printf("Type: %s\tValue: %li\n", lexNames[a.kind], a.id);
        }
        error("Critical failure");
}
