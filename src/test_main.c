#include "ush.h"

int get_line(char **line) {
    int res = 0;

    for(int i = 0; i < 255; i++)
        if(read(0, &(*line)[i], 1) > 0 && i == 0)
            res = 1;
    return res;
}

int main() {
    t_global *head = malloc(sizeof(t_global *));

    char *line = mx_strnew(1024);
    size_t buf = 0;
    char *input = NULL;
    if(isatty(0) == 0 && getline(&line, &buf, stdin) > 0) {
        input = mx_strnew(1024);
        mx_strcpy(input, line);
    }
    else {
        input = mx_strnew(1024);
        mx_strcpy(input, "suka");
    }

    //head->env = mx_env_copy();                  //системні змінні оболонки
    //head->last_exit_status = 0;
    // head->vars = init_vars();                   //локальні змінні оболонки /// ЛИКУЕТ!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //add_var_to_env("SHLVL", "=2", head);
    mx_ush_loop(head, input);                          //основний цикл
    //delete_global(head);                        //НЕ ЧИСТИТЬСЯ, ТРЕБА ЦЮ ФУНКЦІЮ
                                                //ВИКЛИКАТИ ПЕРЕД EXIT
}
