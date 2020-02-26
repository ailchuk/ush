#include "ush.h"

t_lst *lsh_read_line(t_cmd_history **hist) {
    char *line = NULL;
    char **av = NULL;
    t_lst *head = NULL;
    t_lst *tmp = NULL;
    int i = 0;

    line = noncanon_read_line(hist);

    av = ush_split_line(line);      /* розділяємо строку на токени  */

    while (av[i] != NULL) {
        if (mx_is_command(av[i])) {         /* якшо строка це команда, тоді додаємо */
                                            /* команду в список */
            tmp = push_back(&head, av[i]);
            while(av[++i] && strcmp(av[i], "&&") != 0) {   /* доки строка не команда -> записуємо */
                add_new_arg(tmp, av[i]);                            /* строку як аргумент до команди */
            }
        }
        else {
            push_back(&head, av[i]);
            ++i;
        }
    }

    free(line);
    free(av);
    return head;
}

bool mx_is_command(char *str) {
    char **comands = get_commands();            //потрібно потім ЗАФРІШИТИ!!
    
    for(int i = 0; i < COMMANDS; i++) {
        if(strcmp(str, comands[i]) == 0) {
            free(comands);
            return true;
        }
    }
    free(comands);
    return false;
}

char **get_commands() {
    char **comands = (char **)malloc(sizeof(char *) * COMMANDS);

    comands[0] = "export";
    comands[1] = "unset";
    comands[2] = "fg";
    comands[3] = "exit";
    comands[4] = "env";
    comands[5] = "cd";
    comands[6] = "pwd";
    comands[7] = "which";
    comands[8] = "echo";
    comands[9] = "ls";
    return comands;
}

void mx_parse_env_args(t_global **hd) {
    int i;
    char *filename;
    int j = 0;
    char **buf = (char **)malloc(sizeof(char *) * BUFSIZE);

    if(mx_strcmp((*hd)->new->av[1], "-i") == 0) {
        for(i = 2; mx_get_char_index((*hd)->new->av[i], '=') >= 0; i++) ;
        filename = mx_strdup((*hd)->new->av[i]);            //знайшов і зберіг ім'я файла якого треба запустити
        buf[0] = NULL;

        while((*hd)->new->av[++i])
            buf[j++] = mx_strdup((*hd)->new->av[i]);          //зберігаємо флаги якщо вони є

        if((*hd)->new->cmd) {
            mx_del_strarr(&(*hd)->new->av);
            free((*hd)->new->cmd);
        }
        (*hd)->new->cmd = mx_strjoin("/bin/", filename);        //записуємо шляха до бінарника
        (*hd)->new->av = (char **)malloc(sizeof(char *) * 2);   //формуємо масив аргументів
        (*hd)->new->av[0] = mx_strdup(filename);
        (*hd)->new->av[1] = NULL;

        for(j = 0; buf[j]; j++)
            add_new_arg((*hd)->new, buf[j]);
        mx_del_strarr(&buf);
    }
}
