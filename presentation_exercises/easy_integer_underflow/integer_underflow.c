#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char buffer[20];
    int i=atoi(argv[1]);
    memcpy(buffer,argv[2],i*sizeof(int));
    
    printf("the number %d = %d bytes\n",i,i*sizeof(int));
    printf("the buffer is = %s\n",buffer);
    
    return EXIT_SUCCESS;
}
