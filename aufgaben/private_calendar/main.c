#define _GNU_SOURCE
#include <curses.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <crypt.h>

#define BUF_SIZE 24
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define DELWIN(x) \
    do { \
        werase(x); \
        wrefresh(x); \
        delwin(x); \
    } while(0)
#define visible(x, y) (x)->_immed
#define init_ncurses \
    initscr(); \
    cbreak(); \
    noecho(); \
    halfdelay(5); \
    curs_set(0)

volatile sig_atomic_t new_time = 1;
volatile sig_atomic_t run = 1;

void
sig_new_time(
    int sig __attribute__((unused))
) {
    new_time = 1;
}

static int
valid_password(
    const char* pw,
    const size_t len
) {
    if (!len)
        return 0;

    char* tmp = strdup(pw);

    static char right[] =
        // "$6$ebd4e9ca45ebec71$88SHSwN9.XZVSn5RMX3Ua4Y3b195yT3BEs1BJ8PY8XBHFixRM75Nsm2dsg8MzUku9Xi6ca9zB7M/Z.WVMi2XY0";    // asdf
        "$6$ebd4e9ca45ebec71$TJKYrwFgShLImZ0DiJygl8HSGt0m5Ob3ChAOFRbOaMv73LX37QcdHS6l54ZOd8DB2FbNTcM5XduVyz6OI0pry0"; // Hochschulkontaktboerse

    for (size_t i = 0; i < len; ++i)
        tmp[i] ^= 4;

    char* buf = crypt(tmp, "$6$ebd4e9ca45ebec71");
    free(tmp);

    size_t len_buf = strlen(buf);
    if (len_buf != strlen(right))
        return 0;
    for (size_t i = 0; i < len_buf; ++i)
        if (buf[i] != right[i])
            return 0;

    return 1;
}

static WINDOW*
create_main_window(void) {
    static const char title[] = " Private Calendar ";
    unsigned int startpos = COLS/2 - strlen(title)/2;

    WINDOW* mainwin = newwin(LINES, COLS, 0, 0);
    nodelay(mainwin, TRUE);

    box(mainwin, 0, 0);
    mvwaddstr(mainwin, 0, startpos, title);
    wrefresh(mainwin);

    return mainwin;
}

static WINDOW*
create_password_window(void) {
    static const char title[] = " Enter Password ";
    unsigned int height = MIN(LINES, 10);
    unsigned int width = MIN(COLS, 80);
    unsigned int startpos = width/2 - strlen(title)/2;

    WINDOW* pwwin = newwin(height, width, 1, 2);
    keypad(pwwin, TRUE);

    box(pwwin, 0, 0);
    mvwaddstr(pwwin, 0, startpos, title);

    wrefresh(pwwin);
    return pwwin;
}

static WINDOW*
create_login_button(void) {
    unsigned int length = 11;
    WINDOW* loginbutton = newwin(4, length, LINES/2 - 2, COLS/2 - length/2);

    box(loginbutton, 0, 0);
    mvwaddstr(loginbutton, 1, 3, "LOGIN");
    mvwaddstr(loginbutton, 2, 1, "<<ENTER>>");

    wrefresh(loginbutton);
    return loginbutton;
}

static int
login_user(
    WINDOW* mainwin
) {
    WINDOW* pwwin = create_password_window();
    char buf[BUF_SIZE] = { 0 };

    char stars[BUF_SIZE];
    memset(stars, ' ', sizeof stars);
    stars[sizeof stars - 1] = 0;

    int tmp;
    size_t pos = 0;

    while ((tmp = wgetch(pwwin)) != KEY_ENTER && tmp != 10) {
        if (new_time) {
            time_t t;
            struct tm* ti;

            time(&t);
            ti = localtime(&t);
            mainwin->_immed = ti->tm_sec == 4;

            char buf[9] = {0};
            strftime(buf, sizeof buf, "%H:%M:%S", ti);

            mvwaddstr(mainwin, 1, COLS - 10, buf);
            wrefresh(mainwin);
            alarm(1);
        }

        if (pos >= BUF_SIZE) {
            DELWIN(pwwin);
            return 0;
        }

        if (pos && (tmp == KEY_BACKSPACE || tmp == 127)) {
            stars[--pos] = ' ';
            goto print_stars;
        }

        if (!isprint(tmp))
            goto print_stars;

        buf[pos] = tmp;
        stars[pos++] = '*';

print_stars:
        mvwaddstr(pwwin, 4, 13, stars);
        wrefresh(pwwin);
    }

    DELWIN(pwwin);
    return valid_password(buf, pos);
}

static void
create_calendar(
    WINDOW* mainwin
) {
    time_t t;
    struct tm* ti;
    time(&t);
    ti = localtime(&t);

    static const int lengths[] = {1, -1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1};
    mvwprintw(mainwin, 3, COLS/2 - 5, "Year: %d", 1900 + ti->tm_year);
    mvwprintw(mainwin, 5, COLS/2 - 5, "Month: %02d", 1 + ti->tm_mon);

    size_t l = 7;
    size_t c = COLS/2 - 10;
    for (size_t i = 1; i <= (size_t)(30+lengths[ti->tm_mon]); ++i) {
        mvwprintw(mainwin, l, c, "%02d ", i);
        c += 3;
        if (!(i % 7))
            ++l, c = COLS/2 - 10;
    }
    wrefresh(mainwin);
}

int
main(
    int argc,
    char* argv[]
) {
    init_ncurses;
    signal(SIGALRM, sig_new_time);

    WINDOW* mainwin = create_main_window();
    int logged_in = 0;

    while (run) {
        if (new_time) {
            time_t t;
            struct tm* ti;

            time(&t);
            ti = localtime(&t);

            char buf[10] = {0};
            strftime(buf, sizeof buf, "%H:%M:%S", ti);

            mvwaddstr(mainwin, 1, COLS - 10, buf);
            wrefresh(mainwin);
            alarm(1);
        }

        if (logged_in) {
            goto loggedin;
        } else if (!visible(mainwin, TRUE)) {
            WINDOW* login = create_login_button();
            if (wgetch(login) != ERR)
                logged_in = login_user(mainwin);
            DELWIN(login);
            continue;
        }

loggedin:
        create_calendar(mainwin);
        wgetch(mainwin);
        wrefresh(mainwin);
    }

    endwin();
    return 0;
}
