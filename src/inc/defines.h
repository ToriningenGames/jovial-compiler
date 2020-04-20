/**********
 * main.c *
 **********/

//None

/***********
 * error.c *
 ***********/

_Noreturn void error(char *message, ...);

 /*********
  * lex.c *
  *********/

enum lex_type {LEX_NONE, LEX_NUMLIT, LEX_STRLIT, LEX_OP, LEX_IDEN, LEX_SEP};
extern int line;
void newInput(char *name);
