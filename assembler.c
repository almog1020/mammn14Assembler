
#include "preprocess.h"
#include "func.h"

int main(int argc, char* argv[])
{
    int i;
    resrvedNames *rnHead = NULL;
    Line *head = NULL;
    for(i = 1; i < argc; i++){
        /*analyze input..*/
        if(readFile(argv[i],&head)){
		rnHead = pre_process(&head,argv[i]);
	}
	else{
		printf("File: \'%s\' doe's not exists\n",argv[i]);  
		return 0; 
	}   
    }
    if(argc < 2){ 
        printf("No files entered");
	return 0;
    }
    ride1 (head);
    freeResvList(rnHead);
    return 0;
}
