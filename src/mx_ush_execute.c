#include "ush.h"

int mx_find_builtin(t_global *hd, t_lst *head) {
    if (mx_strcmp(head->cmd, "pwd") == 0)
        return mx_builtin_pwd(head); // ret 1
    else if (mx_strcmp(head->cmd, "exit") == 0)
        return mx_exit(hd); // ret 1
    else if (mx_strcmp(head->cmd, "env") == 0)
        return mx_ush_launch(hd, head); // ret 1
    else if(mx_strcmp(head->cmd, "echo") == 0)
        return mx_echo(hd, head);
    else if(mx_strcmp(head->cmd, "export") == 0)
        return mx_export(hd, head);
    else if(mx_strcmp(head->cmd, "cd") == 0)
        return mx_cd(hd, head);
    else if(mx_strcmp(head->cmd, "unset") == 0)
        return mx_unset(hd, head);
    else if(mx_strcmp(head->cmd, "which") == 0)
        return mx_which(hd, head);
    // else if(mx_strcmp(head->cmd, "fg") == 0)
        // return mx_fg(hd, head);
    return 0;
}

static void execute_av_in_qoutes(t_global *hd, t_lst *head) {
    int indx = -1;
    char *str;

    indx = check_cmd_args_for_commands(head);
    if(indx > 0) {
        str = get_cmd_output(head->av[indx], hd);
        free(head->av[indx]);
        head->av[indx] = str;
    }
}
//тут перевірка аргументів на наявність  `cmd` і запуск відповідної ф-кції
//результат віконання цієї ф-кції = строка з виводом запущеної команди
//ця строка записується в якості аргумента поміщеного в ``
int mx_ush_execute(t_global *hd, t_lst *head) {
    int ret = -2;

    //works in general, but makes one root leak, can`t find it 
    execute_av_in_qoutes(hd, head);
    if (head->cmd == NULL)
        return 1;
    ret = mx_find_builtin(hd, head); // 0 launch - 1 builtin
    if (ret < 0) { // -1 error 
        hd->last_exit_status = 1;
        return 1;
    }
    else if (ret > 0) { // builtin
        hd->last_exit_status = 0;
        return 1;
    }
    else if (ret == 0)
        ret = mx_ush_launch(hd, head);
    return ret;
    // return mx_find_builtin(hd, head) ? 1 : mx_ush_launch(hd, head);
    // если нашло билтин то рет 1 иначе лаунч
}

