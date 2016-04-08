#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <crypt.h>
#include <dlfcn.h>


static int member = -1;
static int is_typing = 0;

static const char* pass = "$6$020xKv0F$SlEDM0T3eKvJLyG4o8b2hAdicvbKgcuxE4ztijK9"
                          "wUwUOudkLTOHmFXr0E5mt2TM2iPN869dyReLs0tmKPMY71";
// dd0aac8917a19112b12bc59b02b9080d5f77c43698673121835d6268875e5fea

typedef void (*signal)(int sig, void (*func)(int));

void sig_handler(int num) {
    // If we are not typing then just exit...
    // Or if we shouldn't be typing because no user
    if (!is_typing || member > 0) {
        exit(1);
    }
    // If we are supposed to get the login, we should now stop the typing and
    // let the program gracefully exit itself
    is_typing = 0;
}

int main(int argc, char** argv, char** env) {
    int i;

    signal sig;

    sig = (signal)dlsym(RTLD_NEXT, "signal");
    sig(2, sig_handler);

    for (i = 0; env[i]; i++) {
        // Find the MEMBER env, to get the user number
        char* str = strtok(env[i], "MEMBER=");
        if (str) {
            member = strtol(str, NULL, 10);
        }
    }

    // If there are more than three env variables there are too many arguments
    // Ask the user their id, otherwise don't ask for login
    if (i > 2) {
        printf("Please, enter a userid (or use only $MEMBER to input it): ");
        scanf(" %d", &member);
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Hello %d\n", member);
    is_typing = 1;
    printf("Please enter your password: ");
    char* buffer;
    size_t n = 0;
    getline(&buffer, &n, stdin);
    buffer[strlen(buffer)-1] = 0;

    char* result = crypt(buffer, pass);

    // We check the password and that the user is typing
    if (strcmp(result, pass) != 0 && is_typing == 1) {
        printf("Wrong password\n");
        return -1;
    }

    if (member == 0) {
        printf("Hello Manager, you have logged in!\n");
        return 0;
    }

    printf("Hello user %d you have logged in!\n", member);
    return 0;
}
