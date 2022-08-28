#include "utils.h"

/* reading macros from the linked list and deletes the line they occured at*/
char * read_macro(Line **head, char *macro_name,resrvedNames **rnHead)
{
	Line *temp = *head;
	char *p = NULL;
	int i, j,space_flag,size = 0, lines_to_delete[2] = {0,0}, macro_memdef = 100;
	char temp_line[MAX_LINE], *macro = NULL;
	resrvedNames *temp1 = *rnHead;
	/*reseting temp_line and macro_name*/
	memset(temp_line,0,MAX_LINE);
	memset(macro_name,0,MAX_MACRO_NAME_LEN);
	
	while(temp != NULL)
	{
		/*if endmacro was found before macro*/
		if((p = strstr(temp->line,"endmacro")) && !(isInString(temp->line,"endmacro"))){
		        printf("endmacro inserted without macro\n");
		        p = NULL;
		        break;
		}
		/*finding "macro" in each line*/
		if((p = strstr(temp->line,"macro")) && temp->line[0] != ';' && !(isInString(temp->line,"macro")))
		{
			i = 5;
			j = space_flag = 0;
			/*copying the linked list line to a tempoary string so it will be easier to understand*/
			strcpy(temp_line,p);
			if(!isEmptyBfr(temp->line,"macro")){
				strcpy(macro_name,"ERROR");
				printf("ERROR IN LINE: %d, garbage text before macro\n",temp->og_line);
				return NULL;
			}
			/*no space after "macro"*/
   			if(!isspace(temp_line[i]) && temp_line[i] != '\t')
   			{
        			strcpy(macro_name,"ERROR");
				printf("ERROR IN LINE: %d, no space after macro\n",temp->og_line);
				return NULL;
    			}
    			
			/*skipping white spaces*/
			while(isspace(temp_line[i]) || temp_line[i] == '\t')
				i++;
				
			/*if \n encountered that means that no arguments were inserted afer macro - error*/	
			if(temp_line[i] == '\n')
			{
				strcpy(macro_name,"ERROR");
				printf("ERROR IN LINE: %d, macro name not inserted\n",temp->og_line);
				return NULL;
			}
			
			/*copying macro name to macro_name*/
			while(temp_line[i] != '\n' && temp_line[i] != '\0')
			{
				/*since this loop already starts at char that is not a white space*/
				/*if one is encountered that means that macro_name is already set*/	
				if(isspace(temp_line[i]) || temp_line[i] == '\t')
					space_flag = 1;
				else{
					/*if space_flag is on that means that there is another set of chars*/
					/*after macro name - error*/
					if(!space_flag)
						macro_name[j++] = temp_line[i];
					else
					{
						strcpy(macro_name,"ERROR");
						printf("ERROR IN LINE: %d, too many arguments after macro\n",temp->og_line);
						return NULL;
					}	
				}
				i++;		
			}
			
			macro_name[j] = '\0';	
			/*no need to find another macro reference*/
			lines_to_delete[0] = temp->line_i;
			memset(temp_line,0,MAX_LINE);
			break;
		}
		memset(temp_line,0,MAX_LINE);
		temp = temp->next;
		p = NULL;
	}
	
	/*if no macro was found*/
	if(p == NULL)
		return NULL;
	
	/*if macro name is forbidden*/
	while(temp1 != NULL)
	{
		if(!strcmp(macro_name,temp1->name))
		{
		    /*if macro name is a reserved word*/
		    if(temp1->reserved){
			    strcpy(macro_name,"ERROR");
			    printf("ERROR IN LINE: %d, forbidden macro name\n",temp->og_line);
			    return NULL;
		    }
		    /*else macro name was already defined*/
		    printf("ERROR IN LINE: %d, macro \"%s\" already defined\n",temp->og_line,macro_name);
		    /*if this is enabled no macro will be layout, while this is disabled the latest macro will be layout*/
		    /*strcpy(macro_name,"ERROR");
		    return NULL;*/
		}
		temp1 = temp1->next;
	}
	/*if macro call occoured before macro defenition*/
	temp = *head;
	for(i = 1; i < lines_to_delete[0];i++)
	{
		if((p = strstr(temp->line,macro_name)))
		{
			strcpy(macro_name,"ERROR");
			printf("ERROR IN LINE: %d, macro call before macro defenition\n",temp->og_line);
			return NULL;
		}
		temp = temp->next;	
	}
	temp = temp->next;
	macro = (char*) calloc(macro_memdef, sizeof(char));
	p = NULL;
	
	/*inserting data to macro until endmacro is found*/
	while(!(p = strstr(temp->line,"endmacro"))&& !(isInString(temp->line,"endmacro")))
	{
		strcpy(temp_line,temp->line);
		for(i = 0; i < strlen(temp_line); i++)
		{
			if(size >= macro_memdef)
			{
				macro_memdef *= 2;
				macro = (char*) realloc(macro,macro_memdef * sizeof(char));
			}	
			macro[size++] = temp_line[i];	
		}
		memset(temp_line,0,MAX_LINE);
		temp = temp->next;
	}
	addLastResv(rnHead,0,macro_name);
	macro[size] = '\0';
	/*deleteing lines from macro (including) until endmacro (including)*/
	lines_to_delete[1] = temp->line_i;
	for(j = i = lines_to_delete[0]; j <= lines_to_delete[1];j++)
		deleteLine(head,i);
	
	
	return macro;
} 

/*laying out the macros and deleteing their reference*/
void macro_layout(char *macro, char *macro_name, Line **head)
{
	char *p = NULL, temp_line[MAX_LINE];
	int i, j, line,ma_len = strlen(macro_name);
	Line *temp = *head;
	/*reseting temp_line*/
	memset(temp_line,0,MAX_LINE);
	
	while(temp != NULL)
	{
		/*finding macro name call*/
		if((p = strstr(temp->line,macro_name)) && temp->line[0] != ';' && !(isInString(temp->line,macro_name)))
		{
			/*if p found just a substring that contains macro name*/
			if(!isspace(p[ma_len]) && p[ma_len] != '\t' && p[ma_len] != '\n' && p[ma_len] != EOF){
				temp = temp->next;
				continue;
			}
			if(p != temp->line){
				--p;
				if(!isspace(*p) && *p != '\t'){
					temp = temp->next;
					continue;
				}
			}
			/*marking the line the call was found*/
			line = temp->line_i;
			
			/*if any chars are before or after macro call*/
			if(!isAlone(temp->line,macro_name)){
			        printf("ERROR IN LINE: %d macro call for \"%s\" should be the only thing in the row\n",temp->og_line,macro_name);
			        return;
			    }
 
			for(i = j = 0; j < strlen(macro); i++,j++)
			{
				/*if macro contents contains multiply '\n' each line has to be entered individually*/
				if(macro[j] == '\n'){
					temp_line[i] = macro[j];
					/*inserting line at the desired location*/
					insertNodeAfterLine(line++,temp_line,head);
					/*reseting temp_line*/
					memset(temp_line,0,MAX_LINE);
					/*i is set to minus one becuase it is increased by one at the next iteration which will
					set it to 0*/
					i = -1;	
				}
				else
					temp_line[i] = macro[j];		
			}
			/*deleteing macro call line*/	
			deleteLine(head,temp->line_i);	
		}
	temp = temp->next;
	}	
}

/* reading macros and laying them out in a new "file".am if the macro exists*/
resrvedNames * pre_process(Line **head, char *file_name)
{
	FILE *file;
	char *macro = NULL,macro_name[MAX_MACRO_NAME_LEN];
	int is_macro = 0,i;
	Line *temp = NULL;
	char *badmac_names[31] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get",
	                          "prn","jsr","rts","hlt","r0","r1","r2","r3","r4","r5","r6","r7",".data",
	                          ".struct",".string",".entry",".extern","ERROR","macro"};
	
	resrvedNames *rnHead = NULL;
	
	/*adding badmac_names to the reserved keywords linked list*/
	for(i = 0; i < 31;i++)
	    addLastResv(&rnHead,1,badmac_names[i]);

	/*reading macros*/
	while((macro = read_macro(head,macro_name,&rnHead)))
	{
		/*laying macro in the linked list*/
		macro_layout(macro,macro_name,head);
		is_macro = 1;
		free(macro);
		macro = NULL;
	}
	
	/*if macro is valid without errors, file opening with ".am" extension*/
	if(is_macro && strcmp(macro_name,"ERROR"))
	{
		/*opening file and writing the data to it*/	
		temp = *head;
		strcat(file_name,".am");
		file = fopen(file_name,"w");
		while(temp != NULL)
		{
			for(i = 0; i < strlen(temp->line);i++)
				fputc(temp->line[i],file);
			temp = temp->next;
		}
		fclose(file);
		return rnHead;
	}
	return NULL;
	
	
}
