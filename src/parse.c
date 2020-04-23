#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inc/defines.h"


//Bottom-up parser, because everybody else does recursive-descent
//By the time this file is finished, a full parse tree will have been built.
//Unnecessary, but provides an interesting example
struct parse_node *head;

void parseInit(struct lex_token *items)
{
        head = malloc(sizeof(*head));
        head->item = (struct lex_token){0};
        head->childCount = 0;
        head->siblingRight = NULL;
        head->siblingLeft = NULL;
        head->children = NULL;
        struct lex_token *i;
        struct parse_node *tail;
        for (tail = head, i = items; i->kind; i++) {
                tail->siblingRight = malloc(sizeof(*(tail->siblingRight)));
                tail->siblingRight->siblingLeft = tail;
                tail = tail->siblingRight;
                tail->item = *i;
                tail->childCount = 0;
                tail->children = NULL;
                tail->siblingRight = NULL;
        }
}

//Recursive method of traversal
void deleteNode(struct parse_node *target)
{
        if (target->children) {
                for (int i = 0; i < target->childCount; i++) {
                        deleteNode(target->children[i]);
                }
                free(target->children);
        }
        free(target);
}

void parseEnd()
{
        //Delete the tree
        deleteNode(head);
}

void parsePower()
{
        for (struct parse_node *curr = head; curr; curr = curr->siblingRight) {
                //Is this node relevant?
                if (curr->item.kind != LEX_OP)
                        continue;
                if (curr->item.id != (('*'<<8) | '*'))
                        continue;
                //This is a relevant node
                //Are its arguments valid?
                switch (curr->siblingLeft->item.kind) {
                        case LEX_STRLIT :
                                parseError(curr->siblingLeft, "Cannot exponentiate a string literal");
                        case LEX_SEP :
                                if (curr->siblingLeft->item.id != ')')
                                        parseError(curr->siblingLeft, "Cannot exponentiate this");
                }
                switch (curr->siblingRight->item.kind) {
                        case LEX_STRLIT :
                                parseError(curr->siblingRight, "Cannot use string literal as a power");
                        case LEX_SEP :
                                if (curr->siblingRight->item.id != '(')
                                        parseError(curr->siblingRight, "Cannot exponentiate with this");
                }
                //It will have two children: the nodes to the left and right
                curr->childCount = 2;
                if (!curr->siblingRight)
                        parseError(curr, "Unexpected end of file");
                if (!curr->siblingLeft)
                        parseError(curr, "Unexpected beginning of file");
                curr->children = malloc(sizeof(*(curr->children)) * 2);
                curr->children[0] = curr->siblingLeft;
                curr->children[1] = curr->siblingRight;
                //Remove them from this level
                //Remove ties to them
                if (curr->children[0]->siblingLeft)
                        curr->children[0]->siblingLeft->siblingRight = curr;
                if (curr->children[1]->siblingRight)
                        curr->children[1]->siblingRight->siblingLeft = curr;
                curr->siblingLeft = curr->children[0]->siblingLeft;
                curr->siblingRight = curr->children[1]->siblingRight;
                //Remove thier ties
                curr->children[0]->siblingLeft = NULL;
                curr->children[0]->siblingRight = curr->children[1];
                curr->children[1]->siblingLeft = curr->children[0];
                curr->children[1]->siblingRight = NULL;
        }
}

void parseMulDiv()
{
        for (struct parse_node *curr = head; curr; curr = curr->siblingRight) {
                //Is this node relevant?
                //Looking for '*', '/', 'MOD'
                if (curr->item.kind != LEX_OP) {
                        if (curr->item.kind != LEX_IDEN) {
                                continue;
                        };
                        if (strcmp("MOD", stringTable[curr->item.id])) {
                                continue;
                        };
                } else {
                        if (curr->item.id == (long)'*') {
                                ;       //Don't continue
                        } else if (curr->item.id == (long)'/') {
                                ;       //Don't continue
                        } else {
                                continue;
                        }
                }
                //This is a relevant node
                //Are its arguments valid?
                switch (curr->siblingLeft->item.kind) {
                        case LEX_STRLIT :
                                parseError(curr->siblingLeft, "Expected numeric expression");
                        case LEX_SEP :
                                if (curr->siblingLeft->item.id != ')')
                                        parseError(curr->siblingLeft, "Expected numeric expression");
                }
                switch (curr->siblingRight->item.kind) {
                        case LEX_STRLIT :
                                parseError(curr->siblingRight, "Expected numeric expression");
                        case LEX_SEP :
                                if (curr->siblingRight->item.id != '(')
                                        parseError(curr->siblingRight, "Expected numeric expression");
                }
                //It will have two children: the nodes to the left and right
                curr->childCount = 2;
                if (!curr->siblingRight)
                        parseError(curr, "Unexpected end of file");
                if (!curr->siblingLeft)
                        parseError(curr, "Unexpected beginning of file");
                curr->children = malloc(sizeof(*(curr->children)) * 2);
                curr->children[0] = curr->siblingLeft;
                curr->children[1] = curr->siblingRight;
                //Remove them from this level
                //Remove ties to them
                if (curr->children[0]->siblingLeft)
                        curr->children[0]->siblingLeft->siblingRight = curr;
                if (curr->children[1]->siblingRight)
                        curr->children[1]->siblingRight->siblingLeft = curr;
                curr->siblingLeft = curr->children[0]->siblingLeft;
                curr->siblingRight = curr->children[1]->siblingRight;
                //Remove thier ties
                curr->children[0]->siblingLeft = NULL;
                curr->children[0]->siblingRight = curr->children[1];
                curr->children[1]->siblingLeft = curr->children[0];
                curr->children[1]->siblingRight = NULL;
        }
}

void parseAddSub()
{
        for (struct parse_node *curr = head; curr; curr = curr->siblingRight) {
                //Is this node relevant?
                if (curr->item.kind != LEX_OP)
                        continue;
                if (curr->item.id != '+' && curr->item.id != '-')
                        continue;
                //This is a relevant node
                //Are its arguments valid?
                switch (curr->siblingLeft->item.kind) {
                        case LEX_STRLIT :
                                parseError(curr->siblingLeft, "Expected numeric expression");
                        case LEX_SEP :
                                if (curr->siblingLeft->item.id != ')')
                                        parseError(curr->siblingLeft, "Expected numeric expression");
                }
                switch (curr->siblingRight->item.kind) {
                        case LEX_STRLIT :
                                parseError(curr->siblingRight, "Expected numeric expression");
                        case LEX_SEP :
                                if (curr->siblingRight->item.id != '(')
                                        parseError(curr->siblingRight, "Expected numeric expression");
                }
                //It will have two children: the nodes to the left and right
                curr->childCount = 2;
                if (!curr->siblingRight)
                        parseError(curr, "Unexpected end of file");
                if (!curr->siblingLeft)
                        parseError(curr, "Unexpected beginning of file");
                curr->children = malloc(sizeof(*(curr->children)) * 2);
                curr->children[0] = curr->siblingLeft;
                curr->children[1] = curr->siblingRight;
                //Remove them from this level
                //Remove ties to them
                if (curr->children[0]->siblingLeft)
                        curr->children[0]->siblingLeft->siblingRight = curr;
                if (curr->children[1]->siblingRight)
                        curr->children[1]->siblingRight->siblingLeft = curr;
                curr->siblingLeft = curr->children[0]->siblingLeft;
                curr->siblingRight = curr->children[1]->siblingRight;
                //Remove thier ties
                curr->children[0]->siblingLeft = NULL;
                curr->children[0]->siblingRight = curr->children[1];
                curr->children[1]->siblingLeft = curr->children[0];
                curr->children[1]->siblingRight = NULL;
        }
}

void parseArith()
{
        parsePower();
        parseMulDiv();
        parseAddSub();
}

void parseAll()
{
        parseArith();
}

//Recursive depth-first
void printNode(struct parse_node *curr)
{
        switch (curr->item.kind) {
                case LEX_OP :
                case LEX_SEP :
                        printf("Token: %s\tID: %li\tValue: %c%c\n",
                            lexNames[curr->item.kind],
                            curr->item.id,
                            (char)(curr->item.id & 0xFF),
                            (curr->item.id & 0xFF00) ? (char)(curr->item.id >> 8) : ' ');
                        break;
                case LEX_NUMLIT :
                case LEX_STRLIT :
                case LEX_DBLSTR :
                case LEX_IDEN :
                        printf("Token: %s\tID: %li\tValue: %s\n",
                            lexNames[curr->item.kind],
                            curr->item.id,
                            stringTable[curr->item.id]);
        }
        if (curr->children) {
                printf("Found %i children:\n", curr->childCount);
                for (int i = 0; i < curr->childCount; i++) {
                        printNode(curr->children[i]);
                }
                printf("End of child count for no. %li\n", curr->item.id);
        };
}

void printTree()
{
        printf("\nCurrent parse tree:\n");
        for (struct parse_node *curr = head; curr; curr = curr->siblingRight) {
                printNode(curr);
                printf("Neighboring node:\n");
        }
}
