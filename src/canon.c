#include "ush.h"

void mx_switch_noncanon(struct termios *savetty, struct termios *tty) {
    tcgetattr (0, tty);
    *savetty = *tty; // зберегти управляючу інфy канонічного режиму
    tty->c_lflag &= ~(ICANON|ECHO|ISIG);
    tty->c_cc[VMIN] = 1;
    tcsetattr (0, TCSAFLUSH, tty);
    // з цього моменту канонічний ввід припиняєьться
    // Bимикається echo-вивід на екран і приглушуються всі сигнали
}

void mx_switch_canon(struct termios *savetty) {
    tcsetattr (0, TCSAFLUSH, savetty);
/*          канонічний режим відновлено      */
}

static void wait_next_input(char **line) {
    free(*line);
    *line = mx_strnew(MX_BUFSIZE);
    mx_memset(*line, '\0', MX_BUFSIZE);
    write(1, "u$h> ", 5);
}

static void print_ush(char **line, struct termios savetty) {
    int res = 0;
    bool pressed = false;
    
    for (int i = 0; (res = mx_read_from_stdin(line, &i, &pressed, &savetty));) {
        if (res == MX_LOOP_BREAK) {
            if ((*line)[0] == '\0') {
                mx_printchar('\n');
                wait_next_input(line);
                continue;
            }
            else
                break;
        }
        else if (res == MX_RETURN_EMPTY) {
            wait_next_input(line);
            continue;
        }
        else if (res == MX_LOOP_CONTINUE)
            continue;
    }
}

char *mx_noncanon_read_line(void) {
    struct termios savetty;
    struct termios tty;   
    char *line = mx_strnew(MX_BUFSIZE);


    mx_switch_noncanon(&savetty, &tty);
    mx_memset(line, '\0', MX_BUFSIZE);
    write(1, "u$h> ", 5);
    print_ush(&line, savetty);
    mx_switch_canon(&savetty);
    mx_printchar('\n');
    return line;
}
