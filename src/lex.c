#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "inc/defines.h"
#include "inc/re.h"


int line = 0;
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
        int idsiz;
        if (re_match(reg_identifier, input, &idsiz)) {
                error("Expected identifier");
        };
        char *newid = malloc(idsiz+1);
        strncpy(newid, input, idsiz);
        newid[idsiz] = '\0';
        input += idsiz;
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
        int len;
        struct lex_token out;
        if (re_match(reg_number, input, &len)) {
                error("Expected number");
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
        if (re_match(reg_operator, input, &len)) {
                error("Expected operator");
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
        if (re_match(reg_separator, input, &len)) {
                error("Expected separator");
        };
        out.kind = LEX_SEP;
        out.id = *input++;
        return out;
}

struct lex_token getStr()
{
        struct lex_token out;
        int len;
        if (re_match(reg_string, input, &len)) {
                error("Expected string literal");
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

void getComment()
{
        int len;
        while (re_match(reg_comment, input, &len) || re_match(reg_dblstr, input, &len)) {
                if (re_match(reg_comment, input, &len)) {
                        input += len;
                };
                if (re_match(reg_dblstr, input, &len)) {
                        input += len;
                };
        }
}

struct lex_token getToken()
{
        int len;
        while (!re_match(reg_whitespace, input, &len) || !re_match(reg_comment, input, &len)) {
                if (!re_match(reg_whitespace, input, &len)) {
                        input += len;
                };
                if (!re_match(reg_comment, input, &len)) {
                        input += len;
                };
        }
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
                return (struct lex_token){LEX_NONE, -1};
        } else {
                error("Unknown character '%c'", *input);
        }
}
