#include <stdlib.h>
#include <stdio.h>

void notCalled() {
	char *shell = "/bin/sh";
	char *cmd[] = { "/bin/sh", 0 };
	printf("You´ve got full access to a root shell...\n");
	setreuid(0);
	execve(shell,cmd,0);
}

void vulnerableFunction(char* string) {
	char buffer[100];
	strcpy(buffer, string);
}

int main(int argc, char** argv) {
	vulnerableFunction(argv[1]);
	return 0;
}
