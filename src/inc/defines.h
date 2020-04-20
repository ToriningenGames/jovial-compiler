/**********
 * main.c *
 **********/

//None

/***********
 * error.c *
 ***********/

_Noreturn void error(char *message, ...);

/**********
 * scan.c *
 **********/

extern int line;

 /*********
  * lex.c *
  *********/

enum lex_type {LEX_NONE, LEX_CONST, LEX_OP, LEX_IDEN};
