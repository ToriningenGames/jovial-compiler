#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "inc/defines.h"
#include "inc/re.h"


int lexline = 1;
char *input;


char *stringTable[1024];

//List of regexes to match against
#define reg_whitespace "^\\s+"
#define reg_identifier "^[a-zA-Z$][a-zA-Z0-9$']*"
#define reg_number     "^[+-]?\\d+\\.*\\d*"       //Subset of actual permits
#define reg_operator   "^[=@<>*/+\\-]"            //No two-char ops
#define reg_separator  "^[():,;!]"                //No (* or *)
#define reg_string     "^'[^']*'"               //Escaping ' is done later
//There's funny shenanigans between what are comments and what are define-strings
//Since the whole define portion of the language is a preprocesser in all but name,
//I will be implementing it as such. Thus, it's not a problem here
#define reg_dblstr     "^\"[^\"]\""
#define reg_comment    "^%.*%[^%]"                //% comments only
//Final note on regex:
        //The return value is the index of the match, or -1 if no match was found.
        //Since all our regexes match beginning of string, truthy/falsey values
        //are inverted from normal C


void lexInit()
{
        //Reserved words
        stringTable[ABORT] = "ABORT";
        stringTable[ABS] = "ABS";
        stringTable[AND] = "AND";
        stringTable[BEGIN] = "BEGIN";
        stringTable[BIT] = "BIT";
        stringTable[BITSIZE] = "BITSIZE";
        stringTable[BLOCK] = "BLOCK";
        stringTable[BY] = "BY";
        stringTable[BYREF] = "BYREF";
        stringTable[BYRES] = "BYRES";
        stringTable[BYTE] = "BYTE";
        stringTable[BYTESIZE] = "BYTESIZE";
        stringTable[BYVAL] = "BYVAL";
        stringTable[CASE] = "CASE";
        stringTable[COMPOOL] = "COMPOOL";
        stringTable[CONDITION] = "CONDITION";
        stringTable[CONSTANT] = "CONSTANT";
        stringTable[DEF] = "DEF";
        stringTable[DEFAULT] = "DEFAULT";
        stringTable[DEFINE] = "DEFINE";
        stringTable[ELSE] = "ELSE";
        stringTable[ENCAPSULATION] = "ENCAPSULATION";
        stringTable[END] = "END";
        stringTable[EQV] = "EQV";
        stringTable[EXIT] = "EXIT";
        stringTable[EXPORTS] = "EXPORTS";
        stringTable[FALLTHRU] = "FALLTHRU";
        stringTable[FALSE] = "FALSE";
        stringTable[FIRST] = "FIRST";
        stringTable[FOR] = "FOR";
        stringTable[FREE] = "FREE";
        stringTable[GOTO] = "GOTO";
        stringTable[HANDLER] = "HANDLER";
        stringTable[IF] = "IF";
        stringTable[IN] = "IN";
        stringTable[INLINE] = "INLINE";
        stringTable[INSTANCE] = "INSTANCE";
        stringTable[INTERRUPT] = "INTERRUPT";
        stringTable[ITEM] = "ITEM";
        stringTable[LABEL] = "LABEL";
        stringTable[LAST] = "LAST";
        stringTable[LBOUND] = "LBOUND";
        stringTable[LIKE] = "LIKE";
        stringTable[LOC] = "LOC";
        stringTable[MOD] = "MOD";
        stringTable[NENT] = "NENT";
        stringTable[NEW] = "NEW";
        stringTable[NEXT] = "NEXT";
        stringTable[NOT] = "NOT";
        stringTable[NULL_] = "NULL";
        stringTable[NWDSEN] = "NWDSEN";
        stringTable[OR] = "OR";
        stringTable[OVERLAY] = "OVERLAY";
        stringTable[PARALLEL] = "PARALLEL";
        stringTable[POS] = "POS";
        stringTable[PROC] = "PROC";
        stringTable[PROGRAM] = "PROGRAM";
        stringTable[PROTECTED] = "PROTECTED";
        stringTable[READONLY] = "READONLY";
        stringTable[REC] = "REC";
        stringTable[REF] = "REF";
        stringTable[REGISTER] = "REGISTER";
        stringTable[RENT] = "RENT";
        stringTable[REP] = "REP";
        stringTable[RETURN] = "RETURN";
        stringTable[SGN] = "SGN";
        stringTable[SHIFTL] = "SHIFTL";
        stringTable[SHIFTR] = "SHIFTR";
        stringTable[SIGNAL] = "SIGNAL";
        stringTable[START] = "START";
        stringTable[STATIC] = "STATIC";
        stringTable[STATUS] = "STATUS";
        stringTable[STOP] = "STOP";
        stringTable[TABLE] = "TABLE";
        stringTable[TERM] = "TERM";
        stringTable[THEN] = "THEN";
        stringTable[TO] = "TO";
        stringTable[TRUE] = "TRUE";
        stringTable[TYPE] = "TYPE";
        stringTable[UBOUND] = "UBOUND";
        stringTable[UPDATE] = "UPDATE";
        stringTable[WHILE] = "WHILE";
        stringTable[WITH] = "WITH";
        stringTable[WORDSIZE] = "WORDSIZE";
        stringTable[WRITEONLY] = "WRITEONLY";
        stringTable[XOR] = "XOR";
        stringTable[ZONE] = "ZONE";
        //Predefined values
        stringTable[87] = NULL;
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
        input = name;
}

struct lex_token getID()
{
        //Grab as many qualifying characters as allowed
        //Section 8.2.1 specifies only 31 characters are significant
        struct lex_token out;
        int len;
        out.line = lexline;
        if (re_match(reg_identifier, input, &len)) {
                lexError("Expected identifier");
        };
        char *newid = malloc(len+1);
        strncpy(newid, input, len);
        newid[len] = '\0';
        input += len;
        //Add token to string table
        int i = 0;
        for (i; stringTable[i] && strcmp(newid, stringTable[i]); i++)
                ;
        if (!stringTable[i]) {
                //token not yet encountered
                stringTable[i] = newid;
                stringTable[i+1] = NULL;
        } else {
                //found token
                free(newid);
        }
        //Return token data
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
        //I assume this is covered in parsing
        struct lex_token out;
        int len;
        out.line = lexline;
        if (re_match(reg_number, input, &len)) {
                lexError("Expected number");
        };
        //We shall not try to parse the number now; its type is unknown
        char *newnum = malloc(len+1);
        strncpy(newnum, input, len);
        newnum[len] = '\0';
        input += len;
        //Add token to string table
        int i = 0;
        for (i; stringTable[i] && strcmp(newnum, stringTable[i]); i++)
                ;
        if (!stringTable[i]) {
                //token not yet encountered
                stringTable[i] = newnum;
                stringTable[i+1] = NULL;
        } else {
                free(newnum);
        }
        //Return token data
        out.kind = LEX_IDEN;
        out.id = i;
        return out;
}

struct lex_token getOp()
{
        //Add check
        //The valid punctual ops are outlined in Section 8.2.3
        struct lex_token out;
        int len;
        out.line = lexline;
        if (re_match(reg_operator, input, &len)) {
                lexError("Expected operator");
        };
        out.kind = LEX_OP;
        out.id = *input++;
        //We can't easily get two char operators using our regex subset
        if ((out.id == '*' && *input == '*') ||
            (out.id == '<' && *input == '>') ||
            (out.id == '<' && *input == '=') ||
            (out.id == '>' && *input == '=')) {
                out.id |= *input++ << 8;
        };
        return out;
}

struct lex_token getSep()
{
        //Add check
        //Separators, like ops, are punctuation (Section 8.2.4)
        //We are ignoring (* and *) because they are context dependent :<
        struct lex_token out;
        int len;
        out.line = lexline;
        if (re_match(reg_separator, input, &len)) {
                lexError("Expected separator");
        };
        out.kind = LEX_SEP;
        out.id = *input++;
        return out;
}

struct lex_token getStr()
{
        struct lex_token out;
        int len;
        out.line = lexline;
        if (re_match(reg_string, input, &len)) {
                lexError("Expected string literal");
        };
        out.kind = LEX_STRLIT;
        char *newstr = malloc(len+1);
        strncpy(newstr, input, len);
        newstr[len] = '\0';
        input += len;
        int j = 0;
        for (j; stringTable[j] && strcmp(newstr, stringTable[j]); j++)
                ;
        if (!stringTable[j]) {
                stringTable[j] = newstr;
                stringTable[j+1] = NULL;
        } else {
                free(newstr);
        }
        out.id = j;
        return out;
}

//Clear out any whitespace/comments, and advance the lexline counter as necessary
void getWhitespace()
{
        int len;
        while (isspace(*input) || *input == '"' || *input == '%') {
                if (!re_match(reg_whitespace, input, &len)) {
                        for (; len--; input++) {
                                if (*input == '\n') {
                                        lexline++;
                                };
                        }
                };
                if (!re_match(reg_comment, input, &len)) {
                        for (; len--; input++) {
                                if (*input == '\n') {
                                        lexline++;
                                };
                        }
                };
                if (!re_match(reg_dblstr, input, &len)) {
                        for (; len--; input++) {
                                if (*input == '\n') {
                                        lexline++;
                                };
                        }
                };
        }
}

struct lex_token getToken()
{
        int len;
        getWhitespace();
        if (!re_match(reg_identifier, input, &len)) {
                return getID();
        } else if (!re_match(reg_number, input, &len)) {
                return getNum();
        } else if (!re_match(reg_string, input, &len)) {
                return getStr();
        } else if (!re_match(reg_operator, input, &len)) {
                return getOp();
        } else if (!re_match(reg_separator, input, &len)) {
                return getSep();
        } else if (*input == '\0') {
                return (struct lex_token){LEX_END, -1, lexline};
        } else {
                lexError("Unknown character '%c'", *input);
        }
}
