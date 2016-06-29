#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//If needed, add additional libs

static const char allowedChars[] = "abcdefghijklmnopqrstuvwxyzäöüABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ0123456789!\"§$%&/()=?\\}][{³²|<>,.-_:;'#*+~`´";

void bruteAttack(char* str, int index, int maxDepth, char* progName) {
	int i;
	int exitCode;
	
	for (i = 0; i < sizeof(allowedChars) - 1; ++i) {
        str[index] = allowedChars[i];

        if (index == maxDepth - 1) {
			printf("Test password: %s\n", str);
			//Add one line here and replace the following comment

			/*
			progName = name of the password protected file
			str = contains the current bruteforce pass
			exitCode = contains the Exit Code of the subprocess

			Please create a subprocess which runs the progName with str as argument and note down the exitCode
			If exitCode 0 is returned, the correct password was used
			*/

			//Don´t edit the program here
			if(exitCode == 0) {
				printf("Password found: %s\n", str);
				exit(EXIT_SUCCESS);	
			}
		} else {
			bruteAttack(str, index + 1, maxDepth, progName);
		}
	}
}

void weakBrute(int maxPasswordLength, char* progName) {
    char* buf = malloc(maxPasswordLength + 1); //+1 for '\0'
	int i;
	
    for (i = 1; i <= maxPasswordLength; ++i) {
        memset(buf, 0, maxPasswordLength + 1);
        bruteAttack(buf, 0, i, progName);
    }

    free(buf);
}

int main(int argc, char* argv[]) {
	if(argc == 3) {
		char* progName = malloc(sizeof(char)*strlen(argv[1]));
		strcpy(progName,argv[1]);
		int passwordLength = atoi(argv[2]);
		weakBrute(passwordLength, progName);		
	} else {
		printf("Usage: brute [pass_application] [maxPasswordLength]");
	}
	
	return EXIT_FAILURE;
}
