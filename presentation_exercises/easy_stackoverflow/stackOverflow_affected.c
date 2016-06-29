#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
	char* toAttack = "This content might be overwritten in future\n";
	char input[4];
	
	if(argc == 2) {
		fprintf(stdout,"Normal Content on the Stack-Variable: %s", toAttack);
		fprintf(stdout,toAttack);
		strcpy(input,argv[1]);
		fprintf(stdout,"Your input: %s\n",input);
		fprintf(stdout,"Content overwritten on the Stack-Variable: %s", toAttack);
	} else {
		fprintf(stdout,"an Argument is needed (may be greater than 3 chars)\n");
	}
	return EXIT_SUCCESS;
}
