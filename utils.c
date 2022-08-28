#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"

#define MAX_LINE 81
#define MAX_MACRO_NAME_LEN 30

/*linked list to hold reserved keywords*/
typedef struct resrvedNames{
    int reserved;
    char name[50];
    struct resrvedNames *next;
}resrvedNames;

/*fixes lines after various function ruins their order*/
void fixLineNum(Line **head)
{
    int i = 1;
    Line *temp = *head;
    while(temp != NULL){
        if(temp->line_i != i)
            temp->line_i = i;
        temp = temp->next;
        i++;
    }
}

/* returns the size of the linked list*/
int getSize(Line* head){
    int size = 0;
    Line *temp = head;
    while(temp != NULL){
        temp = temp->next;
        size++;
    }
    return size;
}

/*inserts a struct Line node after the given line_number*/
void insertNodeAfterLine(int line_number, char* line_str, Line** head)
{
    int i, size = getSize(*head);

    Line* newLine = (Line*) calloc(1,sizeof(Line));
    for(i = 0; i < strlen(line_str); i++)
        newLine->line[i] = line_str[i];
    newLine->line_i = line_number+1;
    newLine->og_line = line_number+1;
    newLine->next = NULL;
    
    /*checks out of bonds line number*/
    if(line_number <= 0 || line_number > size){
        return; 
     }   
    /*inserting first node*/
    else if(line_number == 1){
        newLine->next = *head; 
        *head = newLine;
    }
    
    else 
    {
        Line* temp = *head; 
        /*finding desired node*/
        while(--line_number)
            temp=temp->next;
        /*switching positions*/    
        newLine->next= temp->next;
        temp->next = newLine;
    }
    /*fixing damaged lines*/
    fixLineNum(head);
}

/*deletes a desired line*/
void deleteLine(Line** head, int line_number)
{
    Line *temp = *head, *prev;
    int size = getSize(*head);
    
    /*if the line number is out of bond*/
    if(line_number <= 0 || line_number > size) 
        return;
        
    /*if the first line is to be deleted*/
    if (temp != NULL && temp->line_i == line_number) {
        *head = temp->next;
        fixLineNum(head);
        return;
    }
    /*searching for the desired line number while keeping its previous
    on sight for later use*/
    while (temp != NULL && temp->line_i != line_number) {
        prev = temp;
        temp = temp->next;
    }
 
    /* disconnecting node*/
    prev->next = temp->next;
    /*fixing damaged lines*/
    fixLineNum(head);
}


/*add line to the last of the linked list*/
void addLast(Line **head, int i, char *stri)
{
    /*create a new Line*/
    struct Line *newLine = malloc(sizeof(struct Line));
    newLine->line_i = i;
    newLine->og_line = i;
    for(i = 0; i < strlen(stri) ;i++)
        newLine->line[i] = stri[i];
    newLine->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = newLine;
    /*if not empty, find the last Line and add the new Line*/
    else
    {
        Line *lastLine = *head;

        while(lastLine->next != NULL)
            lastLine = lastLine->next;
            
        /*add the new Line at the end of the list*/
        lastLine->next = newLine;
    }
    /*free(newLine);*/
}

/*checks if there are quotes surrouding the wordm like: "word" in the given line, if so retruns 1 else 0*/
int isInString(char *line, char *word)
{
	int i, flag = 0, qutAmount = 0;
	char *p = NULL, temp[81];
	for(i = 0; i < strlen(line); i++){
		temp[i] = line[i];
		if(temp[i] == '\"')
			qutAmount++;
		if(qutAmount % 2 != 0){
			if((p = strstr(temp,word))){
				while(i < strlen(line)){
					if(temp[i++] == '\"'){
						flag = 1;
						break;
					}
					temp[i] = line[i];
				}
			if(flag)
				break;	
			}
		}
	}
	return flag;	
}

/*adding nodes to the reserved words linked list*/
void addLastResv(resrvedNames **head,int reserved, char *name){
     /*create a new Line*/
    int i; 
    struct resrvedNames *newNode = malloc(sizeof(struct resrvedNames));
    newNode->reserved = reserved;
    for(i = 0; i < strlen(name) ;i++)
       newNode->name[i] = name[i];
    newNode->name[i] = '\0';   
    newNode->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = newNode;
    /*if not empty, find the last Line and add the new Line*/
    else
    {
        resrvedNames *lastNode = *head;

        while(lastNode->next != NULL)
            lastNode = lastNode->next;
            
        /*add the new Line at the end of the list*/
        lastNode->next = newNode;
    }
    /*free(newNode);*/
}
/*frees the reserved words linked list*/
void freeResvList(resrvedNames *head)
{
    resrvedNames* temp;
    while (head != NULL)
    {
       temp = head;
       head = head->next;
       free(temp);
       temp=NULL;
    }
}

/*reads the entire file and adds each line to a linked list*/
/*returns 1 if file opend successfully and 0 if not*/
int readFile(char *fileName, Line **head) {
    /*opening file*/
    FILE *file = fopen(fileName, "r");
    long int f_size = 1;
    int counter = 1, c, i = 0;
    char temp[MAX_LINE];
    
    /*reseting temp*/
    memset(temp,0,MAX_LINE);
    /*if file opening failed*/
    if (file == NULL) 
        return 0;
        
    /*analyzing the file size*/    
    fseek(file, 0, SEEK_END);
    f_size += ftell(file);
    fseek(file, 0, SEEK_SET);
    
    /*reading file contents and writing it to "f_context"*/
    while ((c = fgetc(file)) != EOF){
    	/*adding each line to the linked list*/
        if(c != '\n'){
            temp[i++] = (char)c;    
        }
        else{
            temp[i] = (char)c;
            addLast(head,counter++,temp);
            memset(temp,0,81);
            i = 0;
        }
    }
    /*this is needed for windows only*/
    /*addLast(head,counter++,temp);*/
    
    fclose(file);
    return 1;
}

/*checks if there are only white spaces before the word*/
int isEmptyAfr(char *line, char *word){
    char *p, *q = line + strlen(line) - 1;
    p = strstr(line,word);
    /*if word found*/
    if(p){
        /*going forward after the word*/
        p += strlen(word);
        /*if end is met*/
        if(*p == '\0' || *p == '\n')
            return 1;
        /*going forward until the end*/
        while(p != q){
            if(!isspace(*p) && *p != '\t')
                return 0;
        p++;    
        }
        /*final check when p is at the last char*/
        if(!isspace(*p) && *p != '\t')
                return 0;
        return 1;
    }
    return 0;
}

/*checks if there are only white spaces before the word*/
int isEmptyBfr(char *line, char *word){
    char *p, *q = line;
    p = strstr(line,word);
    /*if word found*/
    if(p){
        /*if word is at the begning*/
        if(p == q)
            return 1;
        /*going back before the word*/
        p--;
        /*going back until the first char*/
        while(p != q){
            if(!isspace(*p) && *p != '\t')
                return 0;
        p--;    
        }
        /*final check when p is at the first char*/
        if(!isspace(*p) && *p != '\t')
                return 0;
        return 1;
    }
    return 0;
}

/*checks if the word is the only word in the line*/
int isAlone(char *line, char *word){
    if(isEmptyAfr(line,word) && isEmptyBfr(line,word))
        return 1;
    return 0;    
}
