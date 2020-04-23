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

enum lex_type {LEX_NONE, LEX_NUMLIT, LEX_STRLIT, LEX_DBLSTR, LEX_OP, LEX_IDEN, LEX_SEP, LEX_NEWLINE};
struct lex_token {
        enum lex_type kind;
        long id;
        int line;
};
extern int lexline;
extern char *stringTable[1024];

void lexInit(void);
void newInput(char *name);
struct lex_token getToken(void);

/***********
 * parse.c *
 ***********/

struct parse_node {
        struct lex_token item;
        struct parse_node *siblingRight;
        struct parse_node *siblingLeft;
        struct parse_node **children;
        int childCount; //Immediate children; length of children array
        int position;   //Line number
};

void parseInit(struct lex_token *items);
void parseAll(void);
void printTree(void);

/***********
 * error.c *
 ***********/

_Noreturn void lexError(char *message, ...);
_Noreturn void parseError(struct parse_node *problem, char *message, ...);
