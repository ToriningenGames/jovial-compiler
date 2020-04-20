#include "inc/defines.h"


//Dummy function, to be placed and written later
void newOutput(char *name)
{
        ;
}


int main(int argc, char **argv)
{
        //Windows-specific using standard input as file
        newInput("CON");
        newOutput("CON");
        error("Critical failure");
}
