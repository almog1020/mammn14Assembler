#ifndef utils_h
#define utils_h
#define MAX_LINE 81
#define MAX_MACRO_NAME_LEN 30

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"
/*structs:*/

/*linked list to hold reserved keywords*/
typedef struct resrvedNames{
    int reserved;
    char name[50];
    struct resrvedNames *next;
}resrvedNames;

/*functions:*/

/*inserts a struct Line node after the given line_number*/
void insertNodeAfterLine(int line_number, char* line_str, Line** head);
/*deletes a desired line*/
void deleteLine(Line** head, int line_number);
/*add line to the last of the linked list*/
void addLast(Line **head, int i, char *stri);
/*checks if there are quotes surrouding the word like: "word" in the given line, if so retruns 1 else 0*/
int isInString(char *line, char *word);
/*reads the entire file and adds each line to a linked list*/
/*returns 1 if file opend successfully and 0 if not*/
int readFile(char *fileName, Line **head);
/*adding nodes to the reserved words linked list*/
void addLastResv(resrvedNames **head,int reserved, char *name);
/*frees the reserved words linked list*/
void freeResvList(resrvedNames *head);
/*checks if there are only white spaces before the word*/
int isEmptyAfr(char *line, char *word);
/*checks if there are only white spaces before the word*/
int isEmptyBfr(char *line, char *word);
/*checks if the word is the only word in the line*/
int isAlone(char *line, char *word);
#endif

