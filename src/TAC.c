#include "inc/defines.h"

//TAC output handler
//This file deals with the TAC IR first output by the stack handler
/* Format:
 *      Data sizes and register allocation are not considered.
 *      Values are indexed into a table, linking names with their type and scope
 *      Intermediate values are stored on a stack. The stack may be indexed.
 *      Labels and functions are treated as variables; they are values that can
 *      be assigned to the Program Counter, whose index is always 0
 */

enum var_type {VAR_NONE, VAR_STRING, VAR_INT, VAR_CODE};

struct tacItem {
        ;
};

struct variable {
        const char *ID;
        enum var_type type;
};

void tacPushBlock();
void tacPopBlock();
void tacNewVar();
void tacOperate();
void tacJump();
void tacCall();
void tacLabel();
