#include "ush.h"

char **mx_fill_new_env(t_global **hd, int *i) {
    char **new_env = (char **)malloc(sizeof(char *));
    int j = 0;

    new_env[0] = NULL;

    for (*i = 2; mx_get_char_index((*hd)->new->av[*i], '=') > 0; (*i)++) { 
        while (new_env[j] != NULL)
            j++;
        new_env = realloc(new_env, (sizeof(char *)) * (j + 2));
        new_env[j] = mx_strdup((*hd)->new->av[*i]);
        new_env[j + 1] = NULL;
    }
    return new_env;
}

void mx_find_path_to_cmd(char *folder_to_search, char *filename, 
                            t_global **hd) {
    char *temp = NULL;

    if (folder_to_search == NULL) {
        if (mx_strcmp(filename, "emacs") == 0 
            || mx_strcmp(filename, "vim") == 0 
            || mx_check_if_cmd_is_builtin(filename))
            (*hd)->new->cmd = mx_strjoin("/usr/bin/", filename); 
        else
            (*hd)->new->cmd = mx_strjoin("/bin/", filename);     
    }
    else {  //flag -P
        if (folder_to_search[strlen(folder_to_search) - 1] == '/')
            (*hd)->new->cmd = mx_strjoin(folder_to_search, filename);
        else {
            temp = mx_strjoin(folder_to_search, "/");
            (*hd)->new->cmd = mx_strjoin(temp, filename);
            free(temp);
        }
        free(folder_to_search);
    }
}
//creating new t_lst item to determine command and it's arguments that will be passed to execve
void mx_create_new_cmd_list_item(char *filename, char *folder_to_search, 
                                    t_global **hd) {
    if (filename[0] != '/')
        mx_find_path_to_cmd(folder_to_search, filename, hd);
    else
        (*hd)->new->cmd = mx_strdup(filename);
    (*hd)->new->av = (char **)malloc(sizeof(char *) * 2);  
    (*hd)->new->av[0] = mx_strdup(filename);
    (*hd)->new->av[1] = NULL;
}
//функція правильно розприділяє аргументи команди ENV 
//для подальшої передачі в EXECVE
void mx_reparse_input_for_env(t_global **hd, int i, char *folder_to_search) {
    char *filename;
    char **buf = NULL;
    int j = 0;

    filename = mx_strdup((*hd)->new->av[i]);
    buf = (char **)malloc(sizeof(char *) * MX_BUFSIZE);
    buf[0] = NULL;
    while ((*hd)->new->av[++i])
        buf[j++] = mx_strdup((*hd)->new->av[i]); 
    if ((*hd)->new->cmd) {
        mx_del_strarr(&(*hd)->new->av);
        free((*hd)->new->cmd);
    }
    mx_create_new_cmd_list_item(filename, folder_to_search, hd);
    for (j = 0; buf[j]; j++)
        mx_add_new_arg((*hd)->new, buf[j]);
    mx_del_strarr(&buf);
}
