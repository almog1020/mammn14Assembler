
assembler: assembler.c preprocess.o utils.o func.o 
	gcc -g -ansi -Wall -pedantic preprocess.o utils.o func.o assembler.c -o assembler -lm
preprocess.o: preprocess.c utils.o 
	gcc -c -ansi -Wall -pedantic preprocess.c -o preprocess.o
utils.o: utils.c
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
func.o: func.c
	gcc -c -ansi -Wall -pedantic func.c -o func.o -lm
