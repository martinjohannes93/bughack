#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <crypt.h>

int user_id = -1;
int special_flag = 0;

void sig_func(int sig __attribute__((unused))) {
    /**
     * special_flag == 0bxxxxx0
     *                 0bxxxxx0 ^ 1 == 0bxxxxx1
     *                                 0bxxxxx1 & 1 == 1
     * special_flag == 0bxxxxx1
     *                 0bxxxxx1 ^ 1 == 0bxxxxx0
     *                                 0bxxxxx0 & 1 == 0
     * =>  odd(special_flag) -> true
     * => even(special_flag) -> false
     **/
    if ((user_id <= 0x0) && (((special_flag ^ 1) & 1) == 0))
        special_flag = 0;
    else
        exit(1);
}

int
main(
    int argc,
    char* argv[],
    char* env[]
) {
    // eigentlich ueber dlsym eingebunden
    signal(SIGINT, sig_func);

    // im asm ueber eine iterator variable geloest anstelle einer indexes var
    size_t i = 0;
    while (env[i]) {
        char* tmp = strtok(env[i++], "MEMBER=");
        if (tmp)
            user_id = strtol(tmp, NULL, 10);
    }
    if ((i-1) >= 2) {
        printf("Please, enter a userid (or use only $MEMBER to input it): ");
        scanf("%d", &user_id);
    }
    char tmp;
    do {
        tmp = getchar();
        if (tmp == '\n')
            break;
    } while (tmp != EOF);

    printf("Hello %d\n", user_id);
    special_flag = 1;
    printf("Please enter your password: ");

    char* buf = NULL;
    size_t len = 0;
    getline(&buf, &len, stdin);
    buf[strlen(buf) - 1] = 0;

    if ((strcmp(
                crypt(buf, "$6$020xKv0F$SlEDM0T3eKvJLyG4o8b2hAdicvbKgcuxE4ztijK9wUwUOudkLTOHmFXr0E5mt2TM2iPN869dyReLs0tmKPMY71"),
                "$6$020xKv0F$SlEDM0T3eKvJLyG4o8b2hAdicvbKgcuxE4ztijK9wUwUOudkLTOHmFXr0E5mt2TM2iPN869dyReLs0tmKPMY71")
            != 0) && ((special_flag & 1) != 0)) {
        puts("Wrong password");
        return -1;
    } else {
        if (user_id)
            printf("Hello user %d you have logged in!\n", user_id);
        else
            puts("Hello Manager, you have logged in!");
    }


    return 0;
}
