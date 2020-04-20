#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "inc/defines.h"


static const char *errstr = "Error on line %i: ";

_Noreturn void error(char *message, ...)
{
        va_list args;
        int msglen = snprintf(NULL, 0, errstr, line) + strlen(message) + 1;
        char out[msglen];
        snprintf(out, msglen, errstr, line);
        strcat(out, message);
        va_start(args, message);
        vprintf(out, args);
        va_end(args);
        exit(1);
}
