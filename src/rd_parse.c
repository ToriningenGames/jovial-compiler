#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
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


static struct lex_token next = {0, 0, 1};
static struct lex_token lookahead = {0, 0, 1};

//Special next values
//LEX_NONE for kind accepts any kind of lex item
//-1 for ID accepts any kind of id
//-2 for ID accepts only identifiers
bool anticipate(enum lex_type type, long id)
{
        if (lookahead.kind == LEX_NONE) {
                lookahead = getToken();
        }
        if (next.kind == LEX_NONE) {
                next = lookahead;
                lookahead = getToken();
        };
        if (type != LEX_NONE && next.kind != type)
                return false;
        if (id == -2 && next.id < RESERVEDEND)
                return false;
        if (id > -1 && next.id != id)
                return false;
        return true;
}

//Pulls the next lexing token if it matches input, fails otherwise
struct lex_token expect(enum lex_type type, long id, char *friendlyname)
{
        if (!anticipate(type, id)) {
                switch (next.kind) {
                        case LEX_NUMLIT :
                                parseError(next,
                                    "Expected %s, got a number",
                                    friendlyname);
                        case LEX_STRLIT :
                                parseError(next,
                                    "Expected %s, got a string",
                                    friendlyname);
                        case LEX_OP :
                        case LEX_SEP :
                                parseError(next,
                                    "Expected %s, got '%c%c'",
                                    friendlyname,
                                    next.id & 0xFF,
                                    next.id & 0xFF00 ? next.id >> 8 : ' ');
                        case LEX_IDEN :
                                parseError(next,
                                    "Expected %s, got '%s'",
                                    friendlyname,
                                    stringTable[next.id]);
                        case LEX_END :
                                parseError(next,
                                    "Expected %s, got end of file",
                                    friendlyname);
                }
        };
        struct lex_token temp = next;
        next = lookahead;
        lookahead = getToken();
        return temp;
}

//Sees input, and pulls it if it matches, leaves it otherwise
bool check(enum lex_type type, long id)
{
        if (anticipate(type, id)) {
                expect(type, id, "");
                return true;
        } else {
                return false;
        }
}

void getLabel()
{
        struct lex_token label;
        label = expect(LEX_IDEN, -2, "Label");
        expect(LEX_SEP, ':', "Label's colon");
        tacOut("%s:\n", stringTable[label.id]);
}

void getStatement()
{
        //Zero or more labels
        while (lookahead.kind == LEX_SEP && lookahead.id == ':') {
                getLabel();
        }
        //Either a simple statement or compound
        if (check(LEX_IDEN, BEGIN)) {
                //Compound statement
                //Zero or more statements, followed by zero or more labels
                while (!check(LEX_IDEN, END)) {
                        if (lookahead.kind != LEX_SEP || lookahead.id != ':') {
                                getStatement();
                        };
                        //Because getStatement would die on a label preceding
                            //END, which should be valid
                        while (lookahead.kind == LEX_SEP && lookahead.id == ':') {
                                getLabel();
                        }
                }
        } else {
                //Simple statement, or parse error
                //Check for null statement
                if (check(LEX_SEP, ';')) {
                        return;
                }
                if (check(LEX_IDEN, ABORT)) {
                        //Abort statement
                } else if (check(LEX_IDEN, STOP)) {
                        //Stop statement
                } else if (check(LEX_IDEN, EXIT)) {
                        //Exit statement
                } else if (check(LEX_IDEN, GOTO)) {
                        //Goto statement
                        struct lex_token label = expect(LEX_IDEN, -2, "Label name");
                        expect(LEX_SEP, ';', "semicolon");
                        tacOut("  JMP %s\n", stringTable[label.id]);
                } else if (check(LEX_IDEN, RETURN)) {
                        //Return statement
                        expect(LEX_SEP, ';', "semicolon");
                        tacOut("  RET\n");
                } else if (check(LEX_IDEN, IF)) {
                        //If statement
                } else if (check(LEX_IDEN, CASE)) {
                        //Case statement?!
                } else if (check(LEX_IDEN, FOR)) {
                        //For statement
                } else if (check(LEX_IDEN, WHILE)) {
                        //While statement
                } else {
                        //Either assignment, or procedure call
                        if (!anticipate(LEX_IDEN, -2)) {
                                //Will always fail
                                expect(LEX_IDEN, -2, "statement");
                                assert((bool)"Conflicting info on existence of assignment/procedure");
                        };
                        if (lookahead.kind != LEX_SEP) {
                                expect(LEX_IDEN, -2, "statement");
                                //Will always fail
                                expect(LEX_SEP, '=', "assignment or procedure call");
                                assert((bool)"Conflicting info on existence of assignment/procedure");
                        };
                        if (lookahead.id == '(') {
                                //Procedure call
                        } else if (lookahead.id == ',' || lookahead.id == '=') {
                                //Assignment
                        } else {
                                expect(LEX_IDEN, -2, "statement");
                                //Will always fail
                                expect(LEX_SEP, '=', "assignment or procedure call");
                                assert((bool)"Conflicting info on existence of assignment/procedure");
                        }
                        expect(LEX_SEP, ';', "semicolon");
                }
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
        if (check(LEX_IDEN, COMPOOL)) {
                struct lex_token compname = {LEX_IDEN, -2, 0};
                compname = expect(LEX_IDEN, -2, "compool name");
                tacOut(".DEFINE _COMPOOL_%s\n", stringTable[compname.id]);
                expect(LEX_SEP, ';', "semicolon");
                while ( (0 /*anticipate(/* non-nested subroutine */)) {
                        getCompool();   //TODO
                }
        } else {
                //Are there declaration(s)?
                while ( (0 /*anticipate(/* declarations */)) {
                        ;       //TODO
                }
                //Is there a program?
                if (check(LEX_IDEN, PROGRAM)) {
                        struct lex_token progname = {LEX_IDEN, -2, 0};
                        progname = expect(LEX_IDEN, -2, "program name");
                        expect(LEX_SEP, ';', "semicolon");
                        tacOut(".DEFINE _PROGRAM %s\n", stringTable[progname.id]);
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
        while (anticipate(LEX_IDEN, START))
                getModule();
}
