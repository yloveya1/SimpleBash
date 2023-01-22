#include "s21_grep.h"
struct list * init(char * data) {
    struct list *lst;
    lst = (struct list*)malloc(sizeof(struct list));
    lst->data = data;
    lst->ptr = NULL;
    lst->file = 0;
    return(lst);
}

struct list * addelem(struct list *lst, char *data) {
    struct list *temp, *p;
    temp = (struct list*)malloc(sizeof(struct list));
    p = lst->ptr;
    lst->ptr = temp;
    temp->data = data;
    temp->ptr = p;
    temp->file = 0;
    return(temp);
}

void deletelem(struct list *head) {
    while (head != NULL) {
        struct list *temp = head->ptr;
        if (head->file == 1)
            free(head->data);
        free(head);
        head = temp;
    }
}



int main(int count, char *argv[]) {
    options opt = {0};

    FILE *file = NULL;
    char *str = NULL;
    size_t n = 0, lcount = 0, end = 0;
    int *file_index = calloc(count, sizeof(int));
    struct list* current = init(NULL);
    struct list* head = current;
    if (!file_index) {
        return 1;
    }
    if (init_options(&opt, count, argv, current, file_index, &end) == 0) {
    size_t fcount = 0;
    for (int i = 0; i < count; i++) {
        if (file_index[i] != 0)
            fcount++;
    }
        struct  list* p = head->ptr;
        check_o(&opt, p);
        for (int i = 0; i < count; i++) {
            if (file_index[i] != 0) {
                file = fopen(argv[file_index[i]], "r");
            if (file) {
                p = head->ptr;
                int num = 0;
                ssize_t count_read;
                while ((count_read = getline(&str, &n, file)) > 0) {
                    if (str[count_read - 1] != '\n') {
                        n += 2;
                        char *tmp = realloc(str, n);
                        if (!tmp) {
                            free(str);
                            return 1;
                        } else {
                            str = tmp;
                        }
                        strcat(str, "\n");
                    }
                    int check = 0;
                    num++;
                    while (p != NULL) {
                        find_word(str, p->data, opt, &check);
                        check_v(opt, &check, fcount, &lcount, end, argv[file_index[i]], str, num, p->data);
                        p = p->ptr;
                    }
                    p = head->ptr;
                    if (opt.o == 1 && opt.v == 0) {
                        flag_o(str, p, opt, num, fcount, argv[file_index[i]]);
                    }
                }
                    int k = 0;
                    for (int j = 0; j < k++; j++)
                        p = p->ptr;
                    check_c_and_l(opt, str, fcount, lcount, argv[file_index[i]], p->data);
                    lcount = 0;
                    fclose(file);
            } else if (opt.s == 0) {
                fprintf(stderr, "%s: No such file or directory\n", argv[file_index[i]]);
                break;
            }
        }
        }
    } else if (opt.s == 0) {
        fprintf(stderr, "unknown --directories option");
    }
    deletelem(head);
    free(file_index);
    free(str);
}


int init_options(options *opt, int count, char **argv, struct list* current, int *file_index, size_t *end) {
    int flag = 0;
    int index = 0;
    // type == 1 - NEED PATTERN
    // type == 2 - NEED file_pattern
    int type = 0;

    for (int k = 1; !flag && k < count; k++) {
        if (argv[k][0] == '-' && !type) {
            for (size_t i = 1; !flag && i < strlen(argv[k]); i++) {
                char sim = argv[k][i];
                flag = init_flags(argv[k][i], opt);
                if (!flag) {
                    if (sim == 'e') {
                        if (argv[k][i + 1]) {
                            current = addelem(current, argv[k] + i + 1);
                            (*end)++;
                        } else {
                            type = 1;
                        }
                        break;
                    } else if (sim == 'f') {
                        if (argv[k][i + 1]) {
                            add_pattern_file(&current, argv[k] + i + 1, end);
                        } else {
                            type = 2;
                        }
                        break;
                    }
                }
            }
        } else {
            if (type == 1) {
                current = addelem(current, argv[k]);
                (*end)++;
                type = 0;
            } else if (type == 2) {
                add_pattern_file(&current, argv[k], end);
                type = 0;
            } else {
                file_index[index++] = k;
            }
        }
    }
    flag = flag || type;
    if (!flag &&  opt->e == 0 && opt->f == 0) {
        if (file_index[0]) {
            addelem(current, argv[file_index[0]]);
            file_index[0] = 0;
        }
    }
    return flag;
}

int init_flags(char symb, options *opt) {
    if (symb == 'i')
        opt->i = 1;
    else if (symb == 'v')
        opt->v = 1;
    else if (symb == 'c')
        opt->c = 1;
    else if (symb == 'l')
        opt->l = 1;
    else if (symb == 'n')
        opt->n = 1;
    else if (symb == 'h')
        opt->h = 1;
    else if (symb == 's')
        opt->s = 1;
    else if (symb == 'e')
        opt->e = 1;
    else if (symb == 'f')
        opt->f = 1;
    else if (symb == 'o')
        opt->o = 1;
    else
        return 1;
    return 0;
}

void find_word(char *str, char *find, options opt, int * check) {
    regex_t reg;
    int status = (opt.i == 1) ? regcomp(&reg, find, REG_ICASE) : regcomp(&reg, find, 0);
    if (!status) {
        int status_find = regexec(&reg, str, 0, NULL, 0);
        if (opt.v == 1)
            status_find = !status_find;
        if (!status_find) {
            (*check)++;
        } else if (strcmp(find, ".") == 0 && strcmp(str, "\n") == 0) {
            (*check)++;
        }
    }
    regfree(&reg);
}



void check_c_and_l(options opt, char *str, size_t fcount, size_t lcount, char *file_name, char *pattern) {
    if (opt.l == 1 && opt.c == 1) {
        if (fcount > 1 && opt.h == 0) {
            printf("%s:", file_name);
        }
        (lcount) > 0 ? printf("%d\n", 1) : printf("%d\n", 0);
    } else if (opt.c == 1) {
        if (opt.h == 0 && fcount > 1) {
            printf("%s:%zu\n", file_name, lcount);
        } else {
            printf("%zu\n", lcount);
        }
    }
    if (opt.l == 1) {
        if (lcount > 0) {
            if (strcmp(pattern, ".") == 0) {
                if (strcmp(str, "\n") == 0)
                    printf("%s\n", file_name);
            } else {
                printf("%s\n", file_name);
            }
        }
    }
}


void add_pattern_file(struct list **current, char *file_name, size_t *end) {
    char *str = NULL;
    FILE *file = NULL;
    size_t n;
    file = fopen(file_name, "r");
    if (file) {
        ssize_t count_read;
        while (!feof(file) && (count_read = getline(&str, &n, file)) > 0) {
            if (strlen(str) > 1 && str[strlen(str) - 1] == '\n') {
                str[strlen(str) - 1] = '\0';
            }
            (*end)++;
            char * pattern = malloc(n);
            strcpy(pattern, str);
            *current = addelem(*current, pattern);
            (*current)->file = 1;
        }
        fclose(file);
    }
    free(str);
}

void check_v(options opt, int *check, size_t fc, size_t *lc, int end, char *file, char *str,
    int num, char *patt) {
    if (opt.v == 0) {
        if (*check == 1) {
            if (opt.f == 0 && strcmp(str, "\n") != 0) {
                (*lc)++;
            }
            if (opt.f == 1) {
                (*lc)++;
            }
            print(opt, file, fc, str, num);
            (*check)++;
        }
    } else {
        if (((opt.e == 1 || opt.f == 1) && *check == end) || ((opt.e == 0 && opt.f == 0) && *check == 1)) {
            if (strcmp(patt, ".") == 0) {
                if (strcmp(str, "\n") == 0) {
                    (*lc)++;
                    print(opt, file, fc, str, num);
                }
            } else {
                (*lc)++;
                print(opt, file, fc, str, num);
            }
            (*check)++;
        }
    }
}


void print(options opt, char *argv, size_t fcount, char *str, size_t num) {
    // printf("opt o = %d\n", opt.o);
    // printf("str = %s\n", str);
    if (strcmp(str, "\0") != 0) {
        if (opt.l == 0 && opt.c == 0) {
            if (fcount > 1 && opt.h == 0)
                printf("%s:", argv);
            if (opt.n == 1) printf("%zu:", num);
            if (opt.o == 0 && opt.v == 0 && opt.f != 1) {
                if (strcmp(str, "\n") != 0) {
                    printf("%s", str);
                }
            } else if ((opt.v == 1) || (opt.v == 0 && opt.f == 1 && opt.o == 0)) {
                    printf("%s", str);
            }
            if (strchr(str, '\n') == NULL)
                    printf("\n");
        }
    }
}

void flag_o(char *str, struct list *p_list, options opt, size_t num, size_t fcount, char *filename) {
    size_t i = 0;
    while (p_list != NULL) {
        char *massiv = calloc(strlen(p_list->data) + 1, 1);
        if (massiv) {
            regmatch_t math = {0};
            regex_t reg;
            int status = (opt.i == 1) ? regcomp(&reg, p_list->data, REG_ICASE) :
                regcomp(&reg, p_list->data, 0);
            if (!status) {
                int status_find = 0;
                while (!status_find && i < strlen(str)) {
                    status_find = regexec(&reg, str + i, 1, &math, 0);
                    if (!status_find) {
                        int len = 0;
                        for (int j = math.rm_so; j < math.rm_eo; j++) {
                            massiv[len++] = *(str + i + j);
                        }
                        massiv[len] = '\0';
                        if (opt.c == 0 && opt.l == 0 && strcmp(massiv, "\n") != 0) {
                            printf("%s\n", massiv);
                            flag_o(massiv, p_list->ptr, opt, num, fcount, filename);
                        }
                        i += math.rm_eo;
                    }
                }
            }
            p_list = p_list->ptr;
            free(massiv);
            regfree(&reg);
        }
    }
}

void check_o(options *opt, struct list *p) {
    while (p != NULL) {
        if (strcmp(p->data, "\n") == 0) {
            opt->o = 0;
        }
        p = p->ptr;
    }
}
