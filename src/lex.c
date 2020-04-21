#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "inc/defines.h"
#include "inc/re.h"


int line = 0;
static int lookahead = ' ';
FILE *input;
static char buf[80];

char *stringTable[1024];


int nextChar()
{
        int temp = lookahead;
        do {
                if (lookahead == '\n') {
                        line++;
                };
                lookahead = getchar();
        } while (isspace(lookahead));
        return temp;
}

void lexInit()
{
        //Reserved words
        stringTable[0] = "ABORT";
        stringTable[1] = "ABS";
        stringTable[2] = "AND";
        stringTable[3] = "BEGIN";
        stringTable[4] = "BIT";
        stringTable[5] = "BITSIZE";
        stringTable[6] = "BLOCK";
        stringTable[7] = "BY";
        stringTable[8] = "BYREF";
        stringTable[9] = "BYRES";
        stringTable[10] = "BYTE";
        stringTable[11] = "BYTESIZE";
        stringTable[12] = "BYVAL";
        stringTable[13] = "CASE";
        stringTable[14] = "COMPOOL";
        stringTable[15] = "CONDITION";
        stringTable[16] = "CONSTANT";
        stringTable[17] = "DEF";
        stringTable[18] = "DEFAULT";
        stringTable[19] = "DEFINE";
        stringTable[20] = "ELSE";
        stringTable[21] = "ENCAPSULATION";
        stringTable[22] = "END";
        stringTable[23] = "EQV";
        stringTable[24] = "EXIT";
        stringTable[25] = "EXPORTS";
        stringTable[26] = "FALLTHRU";
        stringTable[27] = "FALSE";
        stringTable[28] = "FIRST";
        stringTable[29] = "FOR";
        stringTable[30] = "FREE";
        stringTable[31] = "GOTO";
        stringTable[32] = "HANDLER";
        stringTable[33] = "IF";
        stringTable[34] = "IN";
        stringTable[35] = "INLINE";
        stringTable[36] = "INSTANCE";
        stringTable[37] = "INTERRUPT";
        stringTable[38] = "ITEM";
        stringTable[39] = "LABEL";
        stringTable[40] = "LAST";
        stringTable[41] = "LBOUND";
        stringTable[42] = "LIKE";
        stringTable[43] = "LOC";
        stringTable[44] = "MOD";
        stringTable[45] = "NENT";
        stringTable[46] = "NEW";
        stringTable[47] = "NEXT";
        stringTable[48] = "NOT";
        stringTable[49] = "NULL";
        stringTable[50] = "NWDSEN";
        stringTable[51] = "OR";
        stringTable[52] = "OVERLAY";
        stringTable[53] = "PARALLEL";
        stringTable[54] = "POS";
        stringTable[55] = "PROC";
        stringTable[56] = "PROGRAM";
        stringTable[57] = "PROTECTED";
        stringTable[58] = "READONLY";
        stringTable[59] = "REC";
        stringTable[60] = "REF";
        stringTable[61] = "REGISTER";
        stringTable[62] = "RENT";
        stringTable[63] = "REP";
        stringTable[64] = "RETURN";
        stringTable[65] = "SGN";
        stringTable[66] = "SHIFTL";
        stringTable[67] = "SHIFTR";
        stringTable[68] = "SIGNAL";
        stringTable[69] = "START";
        stringTable[70] = "STATIC";
        stringTable[71] = "STATUS";
        stringTable[72] = "STOP";
        stringTable[73] = "TABLE";
        stringTable[74] = "TERM";
        stringTable[75] = "THEN";
        stringTable[76] = "TO";
        stringTable[77] = "TRUE";
        stringTable[78] = "TYPE";
        stringTable[79] = "UBOUND";
        stringTable[80] = "UPDATE";
        stringTable[81] = "WHILE";
        stringTable[82] = "WITH";
        stringTable[83] = "WORDSIZE";
        stringTable[84] = "WRITEONLY";
        stringTable[85] = "XOR";
        stringTable[86] = "ZONE";
        //Predefined values
        stringTable[87] = NULL;
        nextChar();
}

void lexEnd()
{
        //Don't try to free the string constants
        for (long i = 87; stringTable[i]; i++) {
                free(stringTable[i]);
                stringTable[i] = NULL;
        }
}

void newInput(char *name)
{
        input = fopen(name, "rt");
}

int peek()
{
        return lookahead;
}

_Bool detect(char *any)
{
        return strchr(any, peek());
}

int expect(char *any)
{
        if (!detect(any)) {
                error("Expected one of '%s'", any);
        };
        return nextChar();
}

struct lex_token getID()
{
        //Grab as many qualifying characters as allowed
        //Section 8.2.1 specifies only 31 characters are significant
        //Names may contain letters, numbers, the $, or the '
        //They must start with a letter or a $
        buf[0] = toupper(expect("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz$"));
        int i = 1;
        for (i; i < 32; i++) {
               if (!detect()) {
                        break;
               };
               buf[i] = expect("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz$'0123456789");
        }
        //Eat whole identifier
        while (detect())
                ;
        if (!isalpha(peek()) && peek() != '$') {
                return (struct lex_token){0,0};
        };
        long i = 0;
        for (i; isalnum(peek()) || peek() == '$' || peek() == '\''; i++) {
                buf[i] = nextChar();
        }
        buf[i] = '\0';
        //Add token to string table
        i = 0;
        for (i; stringTable[i] && strcmp(buf, stringTable[i]); i++)
                ;
        if (!stringTable[i]) {
                //token not yet encountered
                stringTable[i] = malloc(strlen(buf)+1);
                strcpy(stringTable[i], buf);
                stringTable[i+1] = NULL;
        };
        //Return token data
        struct lex_token out;
        out.kind = LEX_IDEN;
        out.id = i;
        return out;
}

struct lex_token getNum()
{
        //Grab as many qualifying characters as allowed
        //Section 8.3.1 is relevant
        //Oddly, the printed form does not permit signs, but the example
                //immediately following uses one.
        //I assume this is an errata,
                //and that signs are allowed in literal contexts
        buf[0] = '\0';
        if (!isdigit(peek())) {
                error("Expected number");
        };
        long i = 0;
        for (i; isalnum(peek()); i++) {
                buf[i] = nextChar();
        }
        buf[i] = '\0';
        //Add token to string table
        i = 0;
        for (i; stringTable[i] && strcmp(buf, stringTable[i]); i++)
                ;
        if (!stringTable[i]) {
                //token not yet encountered
                stringTable[i] = malloc(strlen(buf)+1);
                strcpy(stringTable[i], buf);
                stringTable[i+1] = NULL;
        };
        //Return token data
        struct lex_token out;
        out.kind = LEX_IDEN;
        out.id = i;
        return out;
}

struct lex_token getOp()
{
        //Add check
        //The valid punctual ops are outlined in Section 8.2.3
        struct lex_token out;
        if (!strchr("=@<>*/+-", peek())) {
                error("Expected punctual operator");
        };
        out.kind = LEX_OP;
        out.id = getchar();
        if ((out.id == '*' && peek() == '*') ||
            (out.id == '<' && peek() == '>') ||
            (out.id == '<' && peek() == '=') ||
            (out.id == '>' && peek() == '=')) {
                out.id |= getchar() << 8;
        };
        return out;
}

struct lex_token getSep()
{
        //Add check
        //Separators, like ops, are punctuation (Section 8.2.4)
        //We are ignoring (* and *) because they are context dependent :<
        struct lex_token out;
        if (!strchr("():,;!", peek())) {
                error("Expected separator");
        };
        out.kind = LEX_SEP;
        out.id = getchar();
        return out;
}

struct lex_token getStr()
{
        struct lex_token out;
        switch (nextChar()) {
        case '\'' :
                out.kind = LEX_STRLIT;
                break;
        case '"' :
                out.kind = LEX_DBLSTR;
                break;
        default :
                error("Expected quoted string");
        }
        int i = 0;
        for (i; 1; i++) {
                buf[i] = nextChar();
                if (out.kind == LEX_STRLIT && buf[i] == '\'') {
                        if (peek() == '\'') {
                                nextChar();
                                continue;
                        } else {
                                break;
                        }
                };
                if (out.kind == LEX_DBLSTR && buf[i] == '"') {
                        if (peek() == '"') {
                                nextChar();
                                continue;
                        } else {
                                break;
                        }
                };
        }
        buf[++i] = '\0';
        int j = 0;
        for (j; stringTable[j] && strcmp(stringTable[j], buf); j++)
                ;
        if (!stringTable[j]) {
                stringTable[j] = malloc(strlen(buf));
                strcpy(stringTable[j], buf);
                stringTable[j+1] = NULL;
        };
        out.id = j;
        return out;
}

void getComment()
{
        while (peek() == '%') {
                nextChar();
                while (nextChar() != '%')
                        ;
                if (isspace(peek()))
                        getchar();
        }
}

struct lex_token getToken()
{
#ifdef DEBUG
        printf("Asked to ID '%c'\n", peek());
#endif
        if (isspace(peek())) {
                getchar();
#ifdef DEBUG
                printf("Nevermind, actually a '%c'\n", peek());
#endif
        };
        while (peek() == '%') {
                getComment();
#ifdef DEBUG
                printf("Found a comment\n");
#endif
        }
        if (isalpha(peek())) {
                return getID();
        } else if (isdigit(peek())) {
                return getNum();
        } else if (peek() == '\'' || peek() == '"') {
                return getStr();
        } else if (strchr("=@<>*/+-", peek())) {
                return getOp();
        } else if (strchr("():,;!", peek())) {
                return getSep();
        } else {
                error("Unknown character '%c'", peek());
        }
}
