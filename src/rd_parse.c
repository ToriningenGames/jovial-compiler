#include <stdio.h>
#include <stdarg.h>
#include "inc/defines.h"

//Placeholder
int tacOut(const char *message,  ...)
{
        va_list arg;
        va_start(arg, message);
        int n = vprintf(message, arg);
        va_end(arg);
        return n;
}


static struct lex_token lookahead = {0, 0, 1};

//Special next values
//LEX_NONE for kind accepts any kind of lex item
//-1 for ID accepts any kind of id
//-2 for ID accepts only identifiers
bool anticipate(enum lex_type type, long id)
{
        if (lookahead.kind == LEX_NONE) {
                lookahead = getToken();
        };
        if (type != LEX_NONE && lookahead.kind != type)
                return false;
        if (id == -2 && lookahead.id < RESERVEDEND)
                return false;
        if (id > -1 && lookahead.id != id)
                return false;
        return true;
}

struct lex_token expect(enum lex_type type, long id, char *friendlyname)
{
        if (!anticipate(type, id)) {
                switch (lookahead.kind) {
                        case LEX_NUMLIT :
                                parseError(lookahead,
                                    "Expected %s, got a number",
                                    friendlyname);
                        case LEX_STRLIT :
                                parseError(lookahead,
                                    "Expected %s, got a string",
                                    friendlyname);
                        case LEX_OP :
                        case LEX_SEP :
                                parseError(lookahead,
                                    "Expected %s, got '%c%c'",
                                    friendlyname,
                                    lookahead.id & 0xFF,
                                    lookahead.id & 0xFF00 ? lookahead.id >> 8 : ' ');
                        case LEX_IDEN :
                                parseError(lookahead,
                                    "Expected %s, got '%s'",
                                    friendlyname,
                                    stringTable[lookahead.id]);
                        case LEX_END :
                                parseError(lookahead,
                                    "Expected %s, got end of file",
                                    friendlyname);
                }
        };
        struct lex_token temp = lookahead;
        lookahead = getToken();
        return temp;
}

void getStatement()
{
        //Zero or more labels
        //Either a simple statement or BEGIN
        if (anticipate(LEX_IDEN, BEGIN)) {
                //Compound statement
                expect(LEX_IDEN, BEGIN, "BEGIN");
                //TODO
                expect(LEX_IDEN, END, "END");
        } else {
                //Simple statement (tighen requirements)      TODO
                //Check for null statement
                if (anticipate(LEX_SEP, ';')) {
                        expect(LEX_SEP, ';', "semicolon");
                        return;
                }
                struct lex_token startStmt = expect(LEX_NONE, -1, "anything");
                //TODO
                expect(LEX_SEP, ';', "semicolon");
        }
}

void getCompool()
{
        ;       //TODO
}

void getModule()
{
        expect(LEX_IDEN, START, "START");
        //Is this compool?
        if (anticipate(LEX_IDEN, COMPOOL)) {
                struct lex_token compname = {LEX_IDEN, -2, 0};
                compname = expect(LEX_IDEN, -2, "compool name");
                tacOut("Compool name: %s\n", stringTable[compname.id]);
                expect(LEX_SEP, ';', "semicolon");
                while ( (0 /*anticipate(/* non-nested subroutine */)) {
                        getCompool();   //TODO
                }
        } else {
                //Are there declaration(s)?
                while ( (0 /*anticipate(/* declarations */)) {
                        ;       //TODO
                }
                //Is this a program?
                if (anticipate(LEX_IDEN, PROGRAM)) {
                        expect(LEX_IDEN, PROGRAM, "PROGRAM");
                        struct lex_token progname = {LEX_IDEN, -2, 0};
                        progname = expect(LEX_IDEN, -2, "program name");
                        tacOut("Program name: %s\n", stringTable[progname.id]);
                        expect(LEX_SEP, ';', "semicolon");
                        getStatement();
                };
                //Are there subroutines?
                while ( (0 /*anticipate(/* non-nested subroutine */)) {
                        ;       //TODO
                }
        }
        expect(LEX_IDEN, TERM, "TERM");
}

void getProgram()
{
        getModule();
}
