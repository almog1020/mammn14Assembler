#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "list.h"
#include "func.h"
#define MAX_LINE 81
#define MAX_LINE_LABEL 31
#define BINARY_CODE 10
char* reservedWord[21]= { "mov", "cmp", "add",
  "sub", "lea", "not",
  "clr", "inc", "dec",
  "jmp", "bne", "get",
  "prn", "jsr", "rts",
  "hlt", "data", "string",
  "struct", "entry", "extern"
};
char *comd[21] = { "mov", "cmp", "add",
  "sub", "lea", "not",
  "clr", "inc", "dec",
  "jmp", "bne", "get",
  "prn", "jsr", "rts",
  "hlt", ".data", ".string",
  ".struct", ".entry", ".extern"
};
/*converts a binary numbers string(bin) to imaginary base 32*/
/*returns a string to represent the base 32 number*/
/*when using the function make sure to check the returned result != NULL*/
char * binaryTo32(char *bin)
{
    int k;
    char *result = (char*) calloc(30,sizeof(char));
    char *temp = NULL;
    
    /*first converting the binary to decimal*/
    k = binaryToDecimal(bin);
    /*second converting the decimal to 32*/
    temp = decimalTo32(k);
    strcpy(result,temp);
    free(temp);
    return result;
}

/*converts binary numbers string(bin) to decimal, returns an int as decimal num*/
int binaryToDecimal(char *bin)
{
    int dec = 0, base = 1, r, num = 0,i = 0;
    
    /*passing all left unused bits*/
    while(i < strlen(bin) && bin[i] != '0')
        i++;
    num = atoi(bin);
    
    while (num > 0)  
    {  
        r = num % 10;  
        dec = dec + r * base;  
        num = num / 10;  
        base = base * 2;  
    }
    return dec;
}

/*converts decimal numbers(num) to imaginary base 32*/
/*returns a string to represent the base 32 number*/
/*when using the function make sure to check the returned result != NULL*/
char * decimalTo32(long int num)
{
    /*our imaginary 32 base*/
    char base_arr[32] =
	    {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f','g','h'
	    ,'i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
    int conv_num[10];
    int base = 32, i=0, len = 0;
    char *result = (char*) calloc(30,sizeof(char));
    char c[2], temp;
    
    /*if calloc failed*/
    if(result == NULL)
        return NULL;
    
    /*reseting array*/
    for(; i < 10; i++)
        conv_num[i] = 0;
    i = 0;    

    /* the conversion */
    while (num != 0)
    {
	    conv_num[i] = num % base;
	    num = num / base;
	    i++;
    }
     /* dec "i" since the last element does not exists */
    if(i) 
        --i;
    
    /* traversing the array backwards */
    for(; i>=0; i--)
    {
        c[0] = base_arr[conv_num[i]];
	    strncat(result,c,1);/*adding each "digit" to the result*/
	    len++;
    }
    
    /*null terminator*/
    result[len] = '\0';
    if(strlen(result) <= 1){
        temp = result[0];
        result[0] = '!';
        result[1] = temp;
    }
    return result;	 
}

/*a function that converts decimal to binary*/
/*it returns a pointer to char of a binary represntation, num is the decimal number*/
/*to be converted and bits is the amount of bits to represent the binary outcome*/
/*if int is signed - can only support 8 bit (-128 to 127)*/
/*if int is unsigned - can only support 10 bits(0 to 1023)*/

char *decimalToBinary (int num, int bits){
  unsigned int n = num;
  int i = 0;
  char temp, *result = (char *) calloc ((bits + 1), sizeof (char));
  /*if there is not enough bits to display the number */
  if (abs (num) >= pow (2, bits) || bits > 10 || num <= -512)
    {
      if (bits > 10)
	printf ("max bits is 10\n");
      else if (num <= -512)
	printf ("-511 is the minimum signed value");
      else
	printf ("%d has more bits than %d can provide\n", n, bits);

      free(result);
      return NULL;
    }
  if (num < 0 || num > 1023)
    if (num > 510 || num <= -512){
	free(result);
      	return NULL;
     }
  /*if calloc failed */
  if (result == NULL){
	free(result);
    	return NULL;
  }
  /*assigning bits to *result */
  while (i < bits)
    {
      result[i] = n % 2 + '0';
      n = n / 2;
      i++;
    }
  /*swapping positions from start to end */
  for (i = 0; i < bits / 2; i++)
    {
      temp = result[bits - 1 - i];
      result[bits - 1 - i] = result[i];
      result[i] = temp;
    }
  if (num < 0)
    result[0] = '1';
  /*null terminator */
  result[bits] = '\0';
  return result;

}
/*search label in the symbols table*/
/*return 0 if the symbol is in the table,otherwise return 1*/
int searchLabel (symbols * head, char *testlabel){
  symbols *temp = head;
  while (temp != NULL){
       /*The label in the table*/
      if (strcmp (temp->label, testlabel) == 0)return 0;
      temp = temp->next;
    }
/*The label isnt in the table */
  return -1;
}
/*Check the terms of label*/
/*return ERROR message,otherwise return label*/
char *checkLabel (char *text, int numLine){

  char *label = (char *) calloc (1, 31 * sizeof (char)),*tmp = text;
  int countletter = 0;

    if (strchr (tmp, ':') == NULL){
        printf ("ERROR IN LINE %d:The label=%s must has :\n", numLine, text);
	free(label);
        return NULL;
    }
    if (!isalpha (*tmp)){
        printf("ERROR IN LINE %d:Illegal label name=%s must start with letter\n",numLine, tmp);
	free(label);
        return NULL;
    }
    if (strstr (tmp, ": ") == NULL && strstr (tmp,"\t") == NULL){
        printf("ERROR IN LINE %d:Illegal label name=%s after : must has space\n",numLine, tmp);
	free(label);
        return NULL;
    }
    while (*tmp != ':'){

        if (!isalpha (*tmp) && !isdigit (*tmp)){
	        printf("ERROR IN LINE %d:Illegal label name=%s contain only letters or numbers\n",numLine, tmp);
		free(label);
	        return NULL;
	    }
        countletter++;
        if (countletter > 30){
	        printf ("ERROR IN LINE %d:Illegal label name=%s geater then 30\n",numLine, tmp);
		free(label);
	        return NULL;
	    }
        tmp++;
    }
    strncpy (label, text, countletter);
/*return the label*/
    return label;
}
/*check if the line has label*/
/*add to symbols table,otherwise return ERROR message */
symbols *isLabel (char *text, char *command, int numLine, symbols * symbol, int num,int TypeS){

  char tmp[81] = { '\0' };
  char *label;
  int index;
  while (isspace (*text) || *text == '\t')text++;/*Skips space and taps*/
  strncpy (tmp, text, (int) (strstr (text, command) - text));/*The text before the command*/

  /*check if the label isnt i the middle of the line*/
  if ((strchr (text, ':') - text) > (strstr (text, command) - text)){
      printf("ERROR IN LINE %d:The label in %s need to be in the start of the line\n",numLine, text);
      return NULL;
    }
  if (tmp[0] == '\0')return symbol;/*No label*/ 

  label = checkLabel (tmp, numLine);
  if (label == NULL){
	free(label);
	return NULL;/*The syntax of the label isnt correct*/
  }
  /*The label already in the symbols table*/
  if (searchLabel (symbol, label) == 0){
      free(label);
      printf ("ERROR IN LINE %d:The label=%s already in the symbols table\n",numLine, text);
      return NULL;
    }
  /*Check if the label isnt reserved word*/
  for (index = 0; index < 21; index++){
      if (strcmp (label, reservedWord[index]) == 0){
	        printf ("ERROR IN LINE %d:The label=%s is reserved word\n", numLine,label);
		free(label);
	        return NULL;
	    }
  }
  symbol=insertLabel (symbol, label, num, TypeS);/*return symbols table update*/
  return symbol;
}
/*Insert the label into the symbols table and return the update table*/
symbols * insertLabel (symbols * head, char label[], int num, int TypeS){
  symbols *temp;
  /*New symbols table*/
  if (head == NULL)
    {
      temp = newLabel (label);
      temp->adress = num + 100;
      temp->type = TypeS;
      return temp;
    }
  else
    {
      /*update symbols table*/
      symbols *tmp = head;
      while (tmp->next != NULL)
	            tmp = tmp->next;
      temp = newLabel (label);
      temp->adress = num + 100;
      temp->type = TypeS;
      tmp->next = temp;
    }
  return head;
}
/*Create new symbol*/
symbols * newLabel (char text[]){
  symbols *tmp = NULL;
  tmp = (symbols *) malloc (sizeof (symbols));
  if (tmp != NULL){
      strcpy (tmp->label, text);
      free(text);
      tmp->next = NULL;
    }
  return tmp;
}
/*Print the symbols table*/
void printLabel (symbols * head){
  while (head != NULL)
    {
      printf ("The label:%s,The adress:%d,The type:%d\n", head->label, head->adress,head->type);
      head = head->next;
    }
}

/*Get the command on the line*/
/*return the command,otherwise return ERROR message*/
char * getCommand (char *text,int numLine){

    char *temp, *word = findCommand (text);
    int countL = 0;
  
    if (word == NULL){
      printf ("ERROR IN LINE %d:The line=%s doesnt contain command \n",numLine,text);
      return NULL;
    }
    while (findCommand (text) != NULL){
        temp = strstr (text, findCommand (text));
        text = temp;
        while (*temp != '\0' && !isspace (*temp)){
	            temp++;
	            countL++;
	    }
        if (countL == strlen (word))return word;
        text += countL;
        countL = 0;
        word = findCommand (text);
    }
    return NULL;
}
/*Finding the command in the line*/
/*return the command,otherwise return null*/
char * findCommand (char *text){

    int index, minIndex = 81;
    char *c = NULL;

    for (index = 0; index < 21; index++){
        if (strstr (text, comd[index]) != NULL){
	        if (minIndex > (int) (strstr (text, comd[index]) - text)){
	                minIndex = (int) (strstr (text, comd[index]) - text);
	                c = comd[index];
	        }
	    }
    }
    if (minIndex == 81)return NULL;
    return c;
}
/*Check type line:order,instruction,entry,extern,comment*/ 
/*return the command name,otherwise return ERROR message*/
int typeLine (char *text,int numLine){

    char *command,*tmp = text;

    while (isspace (*tmp) || *tmp == '\t')tmp++;
    if (*tmp == '\0' || strchr(tmp,';')!=NULL)return 1;/*space line or comment line */
  
    command = getCommand (text,0);
    if (command != NULL){
        if (strlen (command) == 3)return 2;/*instruction*/
	    if (strcmp (command, ".entry") == 0)return 4;/*entry*/
	    if (strcmp (command, ".extern") == 0)return 5;/*extern*/
	    return 3;/*order*/        
    }
    printf ("ERROR IN LINE %d:line=%s the command name doesnt correct\n",numLine,text);
    return 0;
}

/*return the index of the command in array comd*/
int indexCpmmand (char txt[]){
	int indexCommand;
    for (indexCommand = 0; indexCommand < 21; indexCommand++){
        if (strcmp (txt, comd[indexCommand]) == 0)
	            return indexCommand;
        }
    return -1;
}
/*validate the instruction sentence*/
/*Add the instruction sentence info and return the up date table,otherwise return ERROR message*/  
tableCode * instruction (char line[], char *nameComand, tableCode * table, int numLine,symbols * symbol){

  int part1, part2, source, destination, indexNameComd =indexCpmmand (nameComand);
  char *binaryCode=(char *) calloc (1, 10 * sizeof (char)),*before = (char *) calloc (1, 81 * sizeof (char)), *after =(char *) calloc (1, 81 * sizeof (char)); 
  char *sentence=strstr (line, nameComand) + 3;
  char* num=decimalToBinary (indexNameComd, 4);
  strcpy(binaryCode,num); 
 /*First group commands*/
  if(indexNameComd>=0 && indexNameComd<=4){
	
    if (sentence[0] == '\0'){
	  printf ("ERROR IN LINE %d:There are no operators in %s \n",numLine,line);
	  free(before);
      	  free(after);
          free(binaryCode);
	  free(num);
	  return NULL;
	}
    if (strchr (sentence, ',') == NULL){
	        printf ("ERROR IN LINE %d:mising , in %s\n",numLine,line);
	        free(before);
            	free(after);
            	free(binaryCode);
		free(num);
	        return NULL;
	}
      strncpy (before, sentence, (int) ((strchr (sentence, ',')) - sentence));/*The text before the command*/
      part1 = checkSentence (before, indexNameComd, 1);
      if (part1 == -1){
          free(before);
          free(after);
          free(binaryCode);
	  free(num);
          return NULL;/*The terms of sentence doesnt correct*/
      }
      /*Binary code*/   
      source = determMethod (before);
      num=decimalToBinary (source, 2);
      strcat(binaryCode,num);
      strcpy(after,strchr (sentence, ','));
      part2 = checkSentence (after, indexNameComd, 2);/*The sentence after ,*/
      if (part2 == -1){
            free(before);
            free(after);
            free(binaryCode);
	    free(num);
          return NULL;/*The terms of sentence doesnt correct*/
      }
      /*Binary code*/    
      destination = determMethod (after);
      num=decimalToBinary (destination, 2);
      strcat(binaryCode,num);

      /*Number of operators*/   
      if ((part2 + part1) != 2){
            free(before);
            free(after);
            free(binaryCode);
	    free(num);
            return NULL;
      }
      if (symbol==NULL)
	        table = insertChar (table, "---", nameComand, sentence, strcat(binaryCode,"00"));
      else{
            while (symbol->next != NULL)symbol = symbol->next;
            table =insertChar (table, symbol->label, nameComand, sentence, strcat(binaryCode,"00"));
      }
      
      if (determMethod (before) == 3 && determMethod (after) == 3){
      
            strcpy(binaryCode,"\0");
      
	        while (!isdigit (*before))before++;
	        strncpy (sentence, before, 1);
		strcpy(before,sentence);
		num=decimalToBinary (atoi (before), 4);
	        strcat(binaryCode,num);
	  
	        while (!isdigit (*after))after++;
	        strncpy (sentence,after, 1);
		strcpy(after,sentence);
		num=decimalToBinary (atoi (after), 4);
	        strcat(binaryCode,num);

	        table=insertChar (table,"---", nameComand,"---",strcat(binaryCode,"00"));
            	free(binaryCode);
		free(num);
		return table;
	  }
      table = binaryCodeOp (table, before, determMethod (before));
      table = binaryCodeOp (table, after, determMethod (after));
	free(num);
   }
      /*Second group commands*/
	else if(indexNameComd>=5 && indexNameComd<=13){
      /*Check the terms of operators*/
      if (checkSentence (sentence, indexNameComd, 2) != 1){
		free(before);
            	free(after);
            	free(binaryCode);
		return NULL;
	}
      /*Binary code*/
      strcat(binaryCode,"00");
      destination = determMethod (sentence);
      num=decimalToBinary (destination, 2);
      strcat(binaryCode,num);
      free(num);
      strcat(binaryCode,"00");
      
      if (symbol==NULL)
	       table = insertChar (table, "---", nameComand, sentence,binaryCode);
      else{
            while (symbol->next != NULL)symbol = symbol->next;
            table =insertChar (table, symbol->label, nameComand, sentence, binaryCode);
      }
      table=binaryCodeOp (table, sentence, determMethod (sentence));
    }
      /*Three group commands*/
      else if(indexNameComd>=14 && indexNameComd<=15){
      /*Check if the line is empty*/
	  while (*sentence!='\0'){
       	     if (!isspace(*sentence)){
			free(before);
            		free(after);
            		free(binaryCode);
           	 printf ("ERROR IN LINE %d:The number of operators didnt match to command in %s \n",numLine,line);
	         return NULL;
	      }
       	      sentence++;
    	  }
        strcat(binaryCode,"000000");
        if (symbol==NULL)
	            table=insertChar (table, "---", nameComand, "---", binaryCode);
        else{
                while (symbol->next != NULL)symbol = symbol->next;
                table=insertChar (table,symbol->label, nameComand, "---",binaryCode);
        }
    }
	free(before);
        free(after);
        free(binaryCode);
        return table;  
}
/*Type of instruction commands on the operators*/
/*Add instruction to the table and return the update table*/
tableCode *binaryCodeOp (tableCode * table, char *word, int type){

  char op[81] = { '\0' };
  int index = 0;
  char* bin,*num = (char *) calloc (1, 1 * sizeof (char)),*b = word;
  
    if (type == 2){
      table = insertChar (table, "---", "---", word, "??????????");
      bin=decimalToBinary(atoi (strchr (b, '.') + 1),8);
	free(num);
      table=insertChar (table, "---", "---", word,strcat(bin,"00"));
	free(bin);
	return table;
    }
    if (type == 1){
	table=insertChar (table, "---", "---", word, "??????????");
	free(num);
	return table;
    }
    while (isspace (*b) || *b == '\t')b++;
    if (type == 3){
      while (!isdigit (*b))b++;
      strncpy (num, b, 1);
      bin=decimalToBinary(atoi(num),4);
      free (num);
      table = insertChar (table, "---", "---", word, strcat(bin,"000000"));
      free (bin);
      return table;
    }
    while (*b != '\0'){
        if (*b == '-' || *b == '+' || isdigit (*b)){
	            op[index] = *b;
	            index++;
	    }
        b++;
    }
    bin=decimalToBinary(atoi (op),10);
    table=insertChar (table, "---", "---", word,bin);
    free(bin);
    free(num);
    return table;
}
/*check line part of instruction*/
/*Return 1 if the term part of instruction is good,otherwish return -1 and ERROR message*/
int checkSentence (char *line, int command, int numPart){

  int numOperator = 0, count = 0;
  char *tmp, *operato = (char *) calloc (1, 81 * sizeof (char));
 while (*line == ',' ){
	line++;
	break;
}
  while (isspace (*line) || *line == '\t' )line++;

    while (*line != '\0'){
      /*Skips spaces and collects the characters*/
        tmp = line;
        while (*tmp != '\0' && !isspace (*tmp)){
	            tmp++;
	            count++;
	    }
        strncpy (operato, line, count);	/*operator to check*/
        if (*operato != '\0'){
	        /*Check word syntax*/
	        if (checkWord (operato, strlen (operato), command, numPart) != -1){
	            numOperator++;
	            strcpy (operato, " ");
	            count = 0;
	            line = tmp;
	            if (((command >= 0 && command < 5) && numOperator > 2)|| ((command > 4 && command < 14) && numOperator > 1)){
		                printf("ERROR:The number of operators didnt match to command in %s \n",line);
		                free(operato);
		                return -1;
		        }
	        }
	        else {
	            free(operato);
	            return -1;/*The word is wrong*/
	        }
	    }
        while (isspace (*tmp) || *tmp == '\t')tmp++;
        line = tmp;
    }
  /*Check whether the word is not empty for case if we dont have space in the end of the word*/
    if (*operato != '\0' && !isspace (*operato)){
            if (checkWord (operato, strlen (operato), command, numPart) != -1)return numOperator + 1;
            if (((command >= 0 && command < 5) && numOperator > 2)|| ((command > 6 && command < 14) && numOperator > 1)){
	                    printf("ERROR:The number of operators didnt match to command in %s \n",line);
	                    free(operato);
	                    return -1;
	        }
    }
	free(operato);
    return numOperator;
}
/*check word without space*/
/*Return 0 if the term word of instruction is good,otherwish return -1 and ERROR message*/
int checkWord (char word[], int sizeWord, int command, int numPart){

  int flageNum = 0;
  char *tmp = word;
  /*Check word syntax*/
  while (*tmp != '\0'){
      /*syntax for #*/
        if (isdigit (*tmp) == 0 && flageNum == 1){
	        printf ("ERROR:The %s doesnt correct because sentence need contain only numbers after # \n",word);
	        return -1;
	    }
      /*syntax for #*/
        if (*tmp == '#'){
	        if (sizeWord == 0){
	                 printf ("ERROR::The %s doesnt correct because after # must have number\n",word);
	                return -1;
	        }
	        if (*(tmp + 1) == '-'){
	                if (*(tmp + 2) == '\0' || (!isdigit (*(tmp + 2)))){
		                printf ("ERROR:The %s doesnt correct because after # must have number\n",word);
		                return -1;
		            }
	        }
	        if (isdigit (*(tmp + 1)) == 0 && *(tmp + 1) != '-'){
	                 printf ("ERROR:The %s doesnt correct because after # must have number\n",word);
	                return -1;
	        }
	        if (numPart == 1 && command > 3){
	             printf ("ERROR:The %s doesnt match to source operator\n",word);
	            return -1;
	        }
	        if (numPart == 2 && (command == 0 || (command > 1 && command < 12)|| (command > 12 && command < 16))){
	                printf ("ERROR:The %s doesnt match to source operator\n",word);
	                return -1;
	        }
	        flageNum = 1;
	        tmp += 2;
	        continue;
	        if (isdigit (*(tmp + 1)) == 0){
	                printf ("ERROR:The %s doesnt correct because after # must have number\n",word);
	                return -1;
	        }
	        flageNum = 1;
	        tmp++;
	        continue;
	    }
      /*syntax for r*/
        if (*tmp == 'r'){
	        if ((sizeWord - 1) != 1){
	                printf ("ERROR:The %s doesnt correct because after the r need be only one number\n",word);
	                return -1;
	        }
	        if (isalpha (*(tmp + 1))){
	             printf ("ERROR:The %s doesnt correct because the word doesnt contain number \n",word);
	            return -1;
	        }
	        if (*(tmp + 1) == '9' || *(tmp + 1) == '0'){
	             printf ("ERROR:The %s doesnt correct because after the r need be number between 1-8\n",word);
	            return -1;
	        }
	        if (numPart == 1 && command > 3){
	             printf ("ERROR:The %s doesnt match to source operator\n",word);
	            return -1;
	        }
	        return 0;
    	}
        tmp++;
    }
    return 0;/*Conditions are normal*/
}
/*determine Method for the operators of instruction*/
int determMethod (char *t){

  while (isspace (*t) || *t == '\t' || *t == ',')t++;
  
  if (*t == '#')return 0;/*number*/

  if (strchr (t, '.') != NULL)return 2;/*label strcut*/

  if (*t == 'r')return 3;/*r*/

  return 1;/*label*/
}
/*validate the order sentence*/
/*Add the order sentence info and return the up date table,otherwise return ERROR message*/  
tableCode * order (char *line, tableCode * table, int numLine, int DC){

  char *b = line;
  char *na = getCommand (b,numLine), *la = checkLabel (line, numLine);
  int pos = indexCpmmand (na), flag = 0, index = 0;
  char *tmp = { '\0' };
  char op[81] = { '\0' };
  char *num;
    /*.data*/
    if (pos == 16){
      b = strstr (b, comd[pos]) + 5;
      while (isspace (*b) || *b == '\t')b++;
      if (checkData (b) == -1){
		free(la);
		return NULL;
	}
	b = strstr (line, comd[pos]) + 5;
         while (*b != '\0'){
	        tmp = b;
	        for(index=0;tmp[index]!='\0' && tmp[index] != ',';index++);
	        strncpy(op,tmp,index);
	        if(atoi (op)>=511 || atoi (op)<=-512){
	                    printf ("ERROR IN LINE %d:The %s need to be between 511 to -512\n", numLine,op);
			    free(la);
	                    return NULL;
	        }
	        if (flag == 0 && index != 0){
	                flag = 1;
			num=decimalToBinary(atoi (op),10);
	                table = insertChar (table, la, na, b,num);
	        }
	        else if (index != 0){
			num=decimalToBinary(atoi (op),10);
			table = insertChar (table, "---", "---", "---",num);
		}
	        while(index>-1){
	            op[index]='\0';
	            index--;
	        }
	        if(strchr(tmp,',')==NULL)
	            break;
	        b=strchr(tmp,',')+1;
	         while (isspace (*b) || *b == '\t')b++;
	        index=0;
	    }
    }
    /*.string*/
    else if (pos == 17){
            b = strstr (b, comd[pos]) + 7;
            while (isspace (*b) || *b == '\t')b++;
            if (checkString (b, numLine) == -1){
			free(la);
			return NULL;
	    }
            b++;
            while (*b != '\0' && *b != '\"'){
	                if (!isspace (*b)){
	                    if (flag == 0){
		                    flag = 1;
				    num=decimalToBinary((int) *b,10);
		                    table = insertChar (table, la, na, b,num);
		                }
	                    else {
				num=decimalToBinary((int) *b,10);
				table = insertChar (table, "---", "---", "---",num);
			    }
	                }
	                b++;
	        }
		num=decimalToBinary(0,10);
	        table = insertChar (table, "---", "---", "---",num);
    }
    /*.struct*/
    else if (pos == 18){
            b = strstr (b, comd[pos]) + 7;
            while (isspace (*b) || *b == '\t')b++;
            while (isspace (*line) || *line == '\t')line++;
            if (checkSentenceStruct (b, numLine) == -1){
			free(la);
			free(na);
			return NULL;
	    }
            while (*b != '\"'){
	                tmp = b;
	                while (*tmp != '\0' && *tmp != ','){
	                        if (!isspace (*tmp)){
		                        op[index] = *tmp;
		                        index++;
		                    }
	                        tmp++;
	                }
	                if (flag == 0 && index != 0){
	                    flag = 1;
			    num=decimalToBinary(atoi (op),10);
	                    table = insertChar (table, la, na, b,num);
	                }
	                else if (index != 0){
				num=decimalToBinary(atoi (op),10);
				table = insertChar (table, "---", "---", "---",num);
			}
	                strcpy (op, " ");
	                index = 0;
	                if (*tmp == ',')tmp++;
	                b = tmp;
	                while (isspace (*b) || *b == '\t')b++;
	        }
            b++;
            while (*b != '\0' && *b != '\"'){
	                if (!isspace (*b)){
				num=decimalToBinary((int) *b,10);
				table = insertChar (table, "---", "---", "---",num);
			}
	                b++;
	        }
		num=decimalToBinary(0,10);
	        table = insertChar (table, "---", "---", "---",num);
		
    }
  return table;
}
/*check operator of .data*/
/*Return operator if the term operator of order is good,otherwish return -1 and ERROR message*/
char * checkOperato (char *check){
  char *c = check;
  while (isspace (*c) || *c == '\t')c++;
    while (*c != '\0'){
         if (isdigit (*c) == 1 && (*c != '+' && *c != '-')){
		if (*c != '\0' && *c != ',')
	                printf ("ERROR::The %s must have , between numbers\n",check);
		else
	          	printf ("ERROR:The %s contain letters\n",c);
	        return NULL;
	    }
        if ((*c == '+' || *c == '-') && isdigit (*(c + 1)) == 0){
	       printf ("ERROR:The %s is missing number after + or -\n", check);
	        return NULL;
	    }
        if (isdigit (*c) != 1 && (*(c + 1) == '+' || *(c + 1) == '-')){
	            printf ("ERROR:The %s is incorrect \n", check);
	            return NULL;
	    }
        c++;
    }
    return c;
}
/*check .data line*/
/*Return 0 if the term data of order is good,otherwish return -1 and ERROR message*/
int checkData (char *data){
  char *word,*mon =data;		
  char operat[81] = { '\0' };
  char operatnew[81] = { '\0' };
  int count = 0,numOperator = 0,i = 0;

    if (*mon == '\0'){
        printf ("ERROR:There are no operators in the line");
        return -1;
    }
    if (*mon == ','){
        printf ("ERROR:The %s have , in the start of line\n", mon);
        return -1;
    }
    while (isspace (*mon) || *mon == '\t')mon++;
    while (*mon != '\0' && *mon!='\n'){
            word = mon;
            while (*word != ',' && *word != '\0' && *word!='\n'){
	                count++;
	                word++;
	        }
            strncpy (operat, mon, count);	/*operator to check*/
            while (isspace (operat[i]) || operat[i] == '\t')i++;
            if (operat[i] != '\0'){
	                strcpy (operatnew, operat);
	                if (checkOperato (operatnew) != NULL){
	                        numOperator++;
	                        strcpy (operatnew, "\0");
	                        count = 0;
				i=0;
	                }
	                else return -1;
	        }
            else{
	            if (*word == ','){
	                    printf ("ERROR:The %s must number between the ,\n", data);
	                    return -1;
	            }
	        }
            word++;
            strcpy(mon,word);
	    while (isspace (*mon) || *mon == '\t')mon++;
    }
    strcpy(mon,data);
    count = 0;
    while (*mon != '\0' && *mon!='\n'){
            if (*mon == ',')count++;
            if (count >= numOperator){
	                printf ("ERROR:The %s have , in the end of line\n", data);
	                return -1;
	        }
            mon++;
    }
    return 0;
}

/*Insert info of the line*/
tableCode *insertChar (tableCode * head, char *lab, char oper[], char operds[], char *ch){
  if (head == NULL)return newChar (lab, oper, operds, ch);
  else{
      tableCode *tmp = head;
      while (tmp->next != NULL)
	            tmp = tmp->next;
      tmp->next = newChar (lab, oper, operds, ch);
    }
  return head;
}
/*Create info of the line*/
tableCode *newChar (char *lab, char oper[], char operds[], char *ch){
  tableCode *tmp = NULL;
  char* opeds=operds;
  tmp = (tableCode *) malloc (sizeof (tableCode));
   while (*opeds == ',' ){
	   opeds++;
	   break;
  }
  if (tmp != NULL)
    {
      strcpy (tmp->label, lab);
      strcpy (tmp->operation, oper);
      strcpy (tmp->operands, opeds);
      strcpy (tmp->binaryMCode, ch);
      tmp->next = NULL;
    }
    return tmp;
}
/*Print info*/
void printTable (tableCode * head){
    tableCode * tmp=head;
    int i;
  while (tmp != NULL){
	printf("label=%s,operation=%s,operands=%s,binaryMCode=",tmp->label,tmp->operation,tmp->operands);
        for(i=0;i<10;i++)printf("%c",tmp->binaryMCode[i]);
        printf("\n___________________\n");
        tmp = tmp->next;
    }
}

/*Count order lines*/
int countData (tableCode * head){
  int count = 0;
  tableCode *p = head;
    while (p != NULL){
        count++;
        p = p->next;
    }
    
  return count;
}
/*Count symbel lines*/
int countSymbel (symbols * head){
  int count = 0;
  symbols *p = head;
    while (p != NULL){
        count++;
        p = p->next;
    }
  return count;
}
/*check .string line*/
/*Return 0 if the term data of order is good,otherwish return -1 and ERROR message*/
int checkString (char *str, int numLine){

  char *sentence = str;

    if (*sentence != '"'){
      printf ("ERROR IN LINE %d:The %s must \" at the start of parameter\n",numLine, str);
      return -1;
    }
    sentence++;

    while (*sentence != '\"' && *sentence != '\0'){
            if (isspace (*sentence)){
	                printf ("ERROR IN LINE %d:The %s need contain letter or numbers\n",numLine, str);
	                return -1;
	        }
            sentence++;
    }
    if (*sentence == '\0'){
            printf ("ERROR IN LINE %d:The %s miss \" in the end \n", numLine, str);
            return -1;
    }
    sentence++;
    while (*sentence != '\0'){
            if (*sentence == '\"'){
	                printf ("ERROR IN LINE %d:The %s has extra \" \n", numLine, str);
	                return -1;
	        }
            if (!isspace (*sentence)){
	                printf ("ERROR IN LINE %d:The %s need to be in the string\n",numLine, str);
                    return -1;
	        }
            sentence++;
    }
    return 0;
}
/*check .struct line*/
/*Return 0 if the term data of order is good,otherwish return -1 and ERROR message*/
int checkSentenceStruct (char *txt, int numLine){
  char *line = txt,*tmp = (char *) calloc (1,81 * sizeof (char));
  int index = 0;

    while (*line != '\0'){
            if (*line == '\"')break;
            
            tmp[index] = *line;
            index++;
            line++;
    }
    if (index == strlen (txt)){
            printf ("ERROR IN LINE %d:The %s must has string",numLine,txt);
            free(tmp);
            return -1;
    }
    if (index == 0){
            printf ("ERROR IN LINE %d:The %s must has num",numLine,txt);
            free(tmp);
            return -1;
    }
    tmp[index] = '\n';
    if (checkData (tmp) == -1){
            free(tmp);
            return -1;
    }
    free(tmp);
    if (checkString (line, numLine) == -1)return -1;
    return 0;
}

/*check label entry And extern*/
/*Return label if the term data of order is good,otherwish return null and ERROR message*/
char * labelEntryAndExtern (char *txt, int numLine, symbols * symbol){

  char *tmp = txt,*label = NULL;
  int countletter = 0, index = 0;

  if (*tmp == '\0'){
      printf ("ERROR IN LINE %d:No label in %s\n",numLine,txt);
      return NULL;
    }
  tmp = txt;
  if (!isalpha (*tmp)){
      printf("ERROR IN LINE: %d, Illegal label name:%s must start with letter\n",numLine, txt);
      return NULL;
    }
  while (*tmp != '\0' && !isspace (*tmp)){
            if (!isalpha (*tmp) && !isdigit (*tmp)){
	                printf("ERROR IN LINE: %d,Illegal label name:%s need contain only letters or numbers\n",numLine, txt);
	                return NULL;
	        }
            countletter++;
            if (countletter > 30){
	                printf("ERROR IN LINE: %d,Illegal label name:  %s geater then 30\n",numLine, txt);
	                return NULL;
	        }
            tmp++;
    }
  label = (char *) calloc (1, 31 * sizeof (char));
  strncpy (label, txt, countletter);

  /*Check if the label isnt reserved word*/
  for (index = 0; index < 21; index++){
        if (strcmp (label, reservedWord[index]) == 0){
	        printf ("ERROR IN LINE %d:The label=%s is reserved word\n", numLine,txt);
	        free (label);
	        return NULL;
	    }
    }
  return label;			/*return the label*/
}
/*Insert label of extern to symbol table,otherwise return ERROR message and null*/
symbols * insertLabelEAE (char *txt, int numLine, symbols * symbol, int DC){
    
  char *comd = getCommand (txt,numLine),*label = NULL,*tmp = txt;
  
  if (strcmp(comd,".entry")==0)
    tmp = strstr (txt, comd) + 6;
  else
    tmp = strstr (txt, comd) + 7;

  while (*tmp != '\0'){
        while (isspace (*tmp) || *tmp == '\t')tmp++;
        label = labelEntryAndExtern (tmp, numLine, symbol);
        if (label == NULL)return NULL;
        if (searchLabel (symbol, label) == 0){
	        printf ("ERROR IN LINE:%d,label in the symbols table\n", numLine);
	        return NULL;
	    }
        if (strcmp (comd, ".extern") == 0)
            symbol = insertLabel (symbol, label, DC, 5);
      else
	        symbol = insertLabel (symbol, label, DC, 4);
      tmp = strchr (tmp, '\0');
    }
  return symbol;
}
/*search label entry in symbol table,change the type and return the update list.otherwise return null*/
symbols *searchLabelEntry (symbols * head, char *testlabel,int num) {
  symbols *temp = head;
  while (temp != NULL){
        if (strcmp (temp->label, testlabel) == 0){
	        temp->type = num;
	        return head;
	    }
        temp = temp->next;
    }
  return NULL;
}
/*First transition algorithm*/
void ride1 (Line *head){

  int type, IC = 0, DC = 0, numLine = 1;
  symbols *symbol = NULL;
  tableCode *table = NULL;
  Line *t=head;

  if (head == NULL)return;
	
  while (t != NULL){
      type = typeLine (t->line,numLine);
      if (type != 1){
	  /*order*/
	  if (type == 3){
	      DC = countData (table);
	      symbol =isLabel (t->line, getCommand (t->line,numLine), numLine, symbol,DC, type);
	      table = order (t->line, table, numLine, DC);
	      if (table == NULL){
			freeList(head);
			return;
		}
	    }
	  /*extern*/
	  else if (type == 5){
	      symbol = insertLabelEAE (t->line, numLine, symbol, -100);
	      if (symbol == NULL){
			freeList(head);
			return;
		}
	    }
	  /*instruction*/
	  else if (type == 2)
	    {
	      symbol=isLabel (t->line, getCommand (t->line,numLine), numLine, symbol,IC, type);
	      if(IC<countSymbel(symbol))
	            table =instruction (t->line, getCommand (t->line,numLine), table,numLine, symbol);
	      else
	            table =instruction (t->line, getCommand (t->line,numLine), table,numLine,NULL);
	      if (table == NULL){
			freeList(head);
			return;
		}
	      IC = countData (table);
	    }
	    
	}
      numLine++;
      t = t->next;
    }
  /*printTable(table);*/
  ride2 (table, symbol,head);
  /*printLabel(symbol);*/
}
/*free a linked list*/    
void freeList( Line* head)
{
   Line* pt=head;
   Line* next;
   while (pt!=NULL)
    {
       next=pt->next;
       free(pt);
       pt=next;
    }
   head=NULL;
}
/*Second transition algorithm*/
void ride2 (tableCode * table, symbols * symbol, Line * head){

  int numLine = 0;
  char *label;
  char* tmp;
  Line* temp=head;

  while (temp != NULL){
      /*.entry*/
      if (typeLine (temp->line,numLine) == 4){
            tmp=temp->line+6;
            while(isspace(*tmp) || *tmp=='\t')tmp++;
	        label = labelEntryAndExtern (tmp, numLine, symbol);
	        if (label == NULL){
			freeList(head);
			return;
		}
	        symbol = searchLabelEntry (symbol, label,4);
	        if (symbol == NULL){
	            printf("ERROR IN LINE:%d,The label=%s isnt in the symbols table\n",numLine, label);
		    freeList(head);
	            return;
	        }
	  }
      numLine++;
      temp = temp->next;
  }
    /*printLabel(symbol);*/
    freeList(head);
    result(upDateTable(table,symbol),symbol);
  return;
}
/*Update the table for second transition algorithm*/
tableCode* upDateTable (tableCode* head,symbols* symbol) {
    
  tableCode* p =head;
  symbols *temp = NULL;
    char *binaryCode;
     while (p != NULL){
         
            if (p->binaryMCode[0] == '?'){
            	 /*serach in label table*/
            		
	           temp=search(symbol,p->operands);
	           if(temp==NULL){
	               printf("%s\n",p->operands);
	               break;
	           }
	           if(temp->type==5)strcpy (p->binaryMCode,"000000001");
                else{
                    binaryCode=decimalToBinary (temp->adress, 8);
                    strcat (binaryCode,"10");
	                strncpy (p->binaryMCode,binaryCode,10);
	                binaryCode="\0";
		        }
                
	        }
        p = p->next;
    }
  return head;
}
/*Search symbol in the symbol table and return the symbol name*/
symbols* search(symbols * head,char* testlabel){
  char str[31];
  char *t=(char *) calloc (1, 1 * sizeof (char));
  char* tmp=testlabel;
  strcpy(str,"\0");
  while(isspace(*tmp) || *tmp=='\t')tmp++;
  while(isalpha(*tmp)||isdigit(*tmp)){
          strncpy(t,tmp,1);
          strcat(str,t);
          tmp++;
  }
    free(t);
    while (head != NULL){
            if( strcmp(str,head->label)==0)return head;/*The label in the table*/
            head = head->next;
    }
  return head;/*The label isnt in the table*/ 
}
/*The result of second and first transitions algorithm*/
void result (tableCode * table, symbols * symbol){
    FILE *fd;
    symbols* freeS;
    tableCode* freet;
    tableCode* t=table;
    int numLine=100;
    char* num;
    if (!(fd = fopen ("ps.ent", "w"))){
            fprintf (stderr, "cannot create file\n");
            exit (0);
    }
    while (symbol != NULL){
            if(symbol->type==4){
                    fprintf (fd, "%s   ",symbol->label);
		    num=decimalTo32 (symbol->adress);
                    fprintf (fd, "%s\n",num);
		    free(num);
            }
	    freeS=symbol;
            symbol = symbol->next;
	    free(freeS);
    }
    if (!(fd = fopen ("ps.ob", "w"))){
      fprintf (stderr, "cannot create file\n");
      exit (0);
    }
    fprintf (fd, "Base32address  Base32Code\n");
    
    while (t != NULL){
	    num=decimalTo32 (numLine);
            fprintf (fd, "    %s   ",num);
	    num=binaryTo32 (t->binaryMCode);
            fprintf (fd, "    %s\n",num);
            numLine++;
            t = t->next;
    }
    free(num);
    if (!(fd = fopen ("ps.ext", "w"))){
      fprintf (stderr, "cannot create file\n");
      exit (0);
    }
    numLine=100;
    while (table != NULL){
            if(strcmp(table->binaryMCode,"000000001")==0){
                fprintf (fd, "%s   ",table->operands);
		num=decimalTo32 (numLine);
                fprintf (fd, "%s\n",num);
		free(num);
            }
        numLine++;
	freet=table;    
        table = table->next;
	free(freet);
    }
    fclose(fd);
    printf("Finish:result in the files\n");
}



