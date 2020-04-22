/**********
 * main.c *
 **********/

//None
void debugOut(int prio, char *format, ...);
extern const char *lexNames[];

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

void lexInit(void);
void newInput(char *name);
struct lex_token getToken(void);

/***********
 * parse.c *
 ***********/

struct parse_node {
        struct lex_token item;
        int childCount; //Immediate children; length of children array
        struct parse_node *siblingRight;
        struct parse_node *siblingLeft;
        struct parse_node **children;
};

void parseInit(struct lex_token *items);
void parseAll(void);
void printTree(void);
