#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void checkArgs(char* argv, char input[], int size) {
	if(strlen(argv) == size) {
		strcpy(input,argv);
	} else {
		fprintf(stderr,"Wrong size of input");
	}
}

int main(int argc, char* argv[]) {
	char input1[4];
	char input2[4];
	
	if(argc == 3) {
		checkArgs(argv[1],input1,strlen(input1));
		checkArgs(argv[2],input2,strlen(input2));
		fprintf(stdout,"Your 1. input: %s\n",input1);
		fprintf(stdout,"Your 2. input: %s\n",input2);
	} else {
		fprintf(stdout,"Wrong number of arguments\n");
	}
		
	return EXIT_SUCCESS;
}
