#ifndef func_h
#define func_h
typedef struct symbols
{
  char label[31];
  int adress;
  int type;
  struct symbols *next;
} symbols;

typedef struct table
{
  char label[31];
  int adress;
  char operation[81];
  char operands[81];
  char binaryMCode[10];
  struct table *next;
} tableCode;

extern char *reservedWord[21]; 
extern char *comd[21];

/*converts decimal numbers(num) to imaginary base 32*/
char * decimalTo32(long int num);
/*converts a binary numbers string(bin) to imaginary base 32*/
char * binaryTo32(char *bin);
/*converts binary numbers string(bin) to decimal, returns an int as decimal num*/
int binaryToDecimal(char *bin);
/*a function that converts decimal to binary*/
char * decimalToBinary(int n, int bits);

/*check if the line has label*/
symbols *isLabel (char *text, char *command, int numLine, symbols * symbol,int DC, int TypeS);
/*Check the terms of label*/
char *checkLabel (char *, int);
/*search label in the symbols table*/
int searchLabel (symbols *, char *);
/*Insert the label into the symbols table and return the update table*/
symbols *insertLabel (symbols * head, char label[], int DC, int TypeS);
/*Create new symbol*/
symbols *newLabel (char text[]);
/*Print the symbols table*/
void printLabel (symbols * head);

/*Finding the command in the line*/
char *findCommand (char *text);
/*Get the command on the line*/
char *getCommand (char *text,int);
/*Check type line:order,instruction,entry,extern,comment*/ 
int typeLine (char *text,int);

/*validate the instruction sentence*/
tableCode *instruction (char line[], char *nameComand, tableCode * table,int numLine, symbols * symbo);
/*check line part of instruction*/
int checkSentence (char *line, int command, int numPart);
/*check word without space*/
int checkWord (char word[], int sizeWord, int command, int numPart);
/*determine Method for the operators of instruction*/
int determMethod (char *word);
/*return the index of the command in array comd*/
int indexCpmmand (char txt[]);

/*validate the order sentence*/
tableCode *order (char *line, tableCode * table, int numLine, int DC);
/*check .data line*/
int checkData (char *data);
/*check operator of .data*/
char *checkOperato (char *check);
/*check .string line*/
int checkString (char *str, int);
/*check .struct line*/
int checkSentenceStruct (char *txt, int);

/*check label entry And extern*/
char *labelEntryAndExtern (char *txt, int numLine, symbols * symbol);
/*Insert label of extern to symbol table*/
symbols *insertLabelEAE (char *txt, int numLine, symbols * symbol, int DC);
/*search label entry in symbol table,change the type*/
symbols *searchLabelEntry (symbols * head, char *testlabel,int num);

/*Print the table*/
void printTable (tableCode * head);
/*Count order lines*/
int countData (tableCode * head);
/*Count symbel lines*/
int countSymbel (symbols * head);

/*Type of instruction commands on the operators*/
tableCode *binaryCodeOp (tableCode * table, char *word, int type);
/*Insert info of the line*/
tableCode *insertChar (tableCode * head, char *lab, char oper[],char operds[], char *ch);
/*Create info of the line*/
tableCode *newChar (char *lab, char oper[], char operds[], char *ch);
/*frees a linked list*/    
void freeList(Line* head);
/*First transition algorithm*/
void ride1 (Line *head);
/*Second transition algorithm*/
void ride2 (tableCode * table, symbols * symbol, Line * head);
/*The result of second and first transitions algorithm*/
void result (tableCode * table, symbols * symbol);
/*Update the table for second transition algorithm*/
tableCode* upDateTable (tableCode* head,symbols* symbol);
/*Search symbol in the symbol table and return the symbol name*/
symbols* search(symbols * head,char* testlabel);

#endif
