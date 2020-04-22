/**********
 * main.c *
 **********/

//None
void debugOut(int prio, char *format, ...);

/***********
 * error.c *
 ***********/

_Noreturn void error(char *message, ...);

 /*********
  * lex.c *
  *********/

enum lex_type {LEX_NONE, LEX_NUMLIT, LEX_STRLIT, LEX_DBLSTR, LEX_OP, LEX_IDEN, LEX_SEP};
struct lex_token {
        enum lex_type kind;
        long id;
};
extern int line;
extern char *stringTable[1024];

void lexInit();
void newInput(char *name);
struct lex_token getToken(void);
