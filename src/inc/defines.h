#include <stdbool.h>

/**********
 * main.c *
 **********/

//None
void debugOut(int prio, char *format, ...);
extern const char *lexNames[];

/*********
 * lex.c *
 *********/

enum lex_type {LEX_NONE, LEX_NUMLIT, LEX_STRLIT, LEX_DBLSTR, LEX_OP, LEX_IDEN, LEX_SEP, LEX_NEWLINE, LEX_END};
struct lex_token {
        enum lex_type kind;
        long id;
        int line;
};
extern int lexline;
extern char *stringTable[1024];
enum res_word {
        ABORT,
        ABS,
        AND,
        BEGIN,
        BIT,
        BITSIZE,
        BLOCK,
        BY,
        BYREF,
        BYRES,
        BYTE,
        BYTESIZE,
        BYVAL,
        CASE,
        COMPOOL,
        CONDITION,
        CONSTANT,
        DEF,
        DEFAULT,
        DEFINE,
        ELSE,
        ENCAPSULATION,
        END,
        EQV,
        EXIT,
        EXPORTS,
        FALLTHRU,
        FALSE,
        FIRST,
        FOR,
        FREE,
        GOTO,
        HANDLER,
        IF,
        IN,
        INLINE,
        INSTANCE,
        INTERRUPT,
        ITEM,
        LABEL,
        LAST,
        LBOUND,
        LIKE,
        LOC,
        MOD,
        NENT,
        NEW,
        NEXT,
        NOT,
        NULL_,
        NWDSEN,
        OR,
        OVERLAY,
        PARALLEL,
        POS,
        PROC,
        PROGRAM,
        PROTECTED,
        READONLY,
        REC,
        REF,
        REGISTER,
        RENT,
        REP,
        RETURN,
        SGN,
        SHIFTL,
        SHIFTR,
        SIGNAL,
        START,
        STATIC,
        STATUS,
        STOP,
        TABLE,
        TERM,
        THEN,
        TO,
        TRUE,
        TYPE,
        UBOUND,
        UPDATE,
        WHILE,
        WITH,
        WORDSIZE,
        WRITEONLY,
        XOR,
        ZONE};

void lexInit(void);
void newInput(char *name);
struct lex_token getToken(void);

#define RESERVEDEND 87  //First identifier in the stringTable

/***********
 * parse.c *
 ***********/

void getProgram();

/***********
 * error.c *
 ***********/

_Noreturn void lexError(char *message, ...);
_Noreturn void parseError(struct lex_token problem, char *message, ...);

