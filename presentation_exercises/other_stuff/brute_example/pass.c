#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* secretPass = "bb";

int main(int argc, char* argv[]) {
	if(argc == 2) {		
		if(strcmp(secretPass,argv[1]) == 0) {
			printf("Login successful!");
			
			return EXIT_SUCCESS;
		}
	}
	
	return EXIT_FAILURE;
}
