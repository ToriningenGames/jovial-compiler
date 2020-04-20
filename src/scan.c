#include <stdio.h>
#include "inc/defines.h"


int line = 0;
static int lookahead = ' ';

int nextchar()
{
        int temp = lookahead;
        lookahead = getchar();
        return temp;
}

void expect(int n)
{
        if (nextchar() != n) {
                error("Expected '%c'", n);
        };
}
