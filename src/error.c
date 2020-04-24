#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "inc/defines.h"


static const char *lexerrstr = "Lexing error on line %i: ";
static const char *parseerrstr = "Parse error on line %i: ";

_Noreturn void lexError(char *message, ...)
{
        va_list args;
        int msglen = snprintf(NULL, 0, lexerrstr, lexline) + strlen(message) + 1;
        char out[msglen];
        snprintf(out, msglen, lexerrstr, lexline);
        strcat(out, message);
        va_start(args, message);
        vprintf(out, args);
        va_end(args);
        exit(1);
}

_Noreturn void parseError(struct lex_token problem, char *message, ...)
{
        va_list args;
        int msglen = snprintf(NULL, 0, parseerrstr, problem.line) + strlen(message) + 1;
        char out[msglen];
        snprintf(out, msglen, parseerrstr, problem.line);
        strcat(out, message);
        va_start(args, message);
        vprintf(out, args);
        va_end(args);
        exit(1);
}
