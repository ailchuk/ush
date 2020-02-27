#include "ush.h"

bool mx_match(char *src, char *regex) {
    regex_t reg;
    int result;

    regcomp(&reg, regex, REG_EXTENDED);
    result = regexec(&reg, src, 0, NULL, 0);
    regfree(&reg);
    return result == 0;
}

static int print_exec_error(t_lst *new) {
    if (!mx_match(new->cmd, "/")) {
        fprintf(stderr, "ush: %s: Command not found\n", new->cmd);
        return 127;
    }
    else {
        for (int i = 0; new->av[i]; i++)
            if (mx_match(new->av[i], "/"))
                fprintf(stderr, "ush: %s: No such file or SUKA directory\n", new->av[i]);
    }
    return 127;
}

static int func_exec(t_global *hd) {
    int status = 0;
    char **new_env;

    if (mx_strcmp(hd->new->cmd, "env") == 0) {
        new_env = mx_parse_env_args(&hd);       //функція повертає NULL якщо в env не передано ніяких команд
        if(new_env == NULL)                     //інакше - повертає масив змінних среди
            return 1;
        else
            status = execve(hd->new->cmd, hd->new->av, new_env); //запускаємо env з заданим набором змінних
        mx_del_strarr(&new_env);
        printf("****1\t %d\n", status);
    }
    else {
        status = execvp(hd->new->cmd, hd->new->av); // МОЖЕТ ЛУЧШЕ execvp? при таком случае работает лс и тд юзаємо с новими аргументами середи
        printf("****2\t %d\n", status);
        if (status == -1) {
            print_exec_error(hd->new);
        }
    }

    return status;
}

int mx_ush_launch(t_global *hd) {
    pid_t pid;          //proccess id
    pid_t wpid;         //
    int status;
    int res = 0;

    pid = fork();       //створюємо процес і зберігаємо значення
//як тільки fork() повертає значення, ми отримуємо два паралельних процеса
//fork повертає нуль для child proccess і id процесу для батьківського процесу
    if(pid == 0) {      //нуль повертається child proccess
        //дочерний процес
        /*
            exec() - системний виклик, який дозволяє виконати вбудувані команди
            оболонки шел поза процесом самого шел
            v -> ми передаємо вектор (масив строк)
            p -> замість повного шляху до команди ми передаємо тільки її імя
        */

        if((res = func_exec(hd)) == 1) {
            exit(1);
        }
        if(res == 127)
            exit(0);
    }
    // if(pid < 0)
        // error
    wpid = waitpid(pid, &status, WUNTRACED);
    return 1;
}
