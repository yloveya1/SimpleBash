#include "s21_cat.h"

int main(int count, char *argv[]) {
    int a = 1, b = 1, check = 0;
    options opt = {0};
    FILE *file;
    char *str = NULL;
    size_t n = 0;
    for (int i = 1; i < count; i++) {
        if (argv[i][0] == '-' && check == 0)  {
            init_options(&opt, argv[i]);
        } else {
            file = fopen(argv[i], "r");
            if (file) {
                ssize_t count_read;
                check = 1;
                while (!feof(file) && (count_read = getline(&str, &n, file)) > 0) {
                    if (opt.s == 1) {
                        if (strcmp(str, "\n") == 0) {
                            while (((count_read = getline(&str, &n, file)) > 0) && strcmp(str, "\n") == 0) {
                            }
                            fseek(file, -count_read - 1, 1);
                            count_read = getline(&str, &n, file);
                            if (count_read < 0) {
                                strcpy(str, "\n");
                                count_read = 1;
                            }
                        }
                    }
                    if (opt.n == 1 && opt.b != 1) {
                        opt_n(a);
                        a++;
                    }
                    if (opt.b == 1) {
                        opt_b(str, &b);
                    }
                    print_file(opt, str, count_read);
                }
                fclose(file);
                a = b = 1;
            }  else {
                fprintf(stderr, "%s: No such file or directory", argv[i]);
            }
        }
    }
    free(str);
}

void init_options(options *opt, char *argv) {
    for (size_t i = 1; i < strlen(argv); i++) {
            if (strchr("nevbstTE", argv[i]) && argv[1] != '-') {
                if (argv[i] == 'b')
                    opt->b = 1;
                if (argv[i] == 'E')
                    opt->e = 1;
                if (argv[i] == 'e') {
                    opt->e = 1;
                    opt->v = 1;
                }
                if (argv[i] == 'v')
                    opt->v = 1;
                if (argv[i] == 'n')
                    opt->n = 1;
                if (argv[i] == 's')
                    opt->s = 1;
                if (argv[i] == 'T')
                    opt->t = 1;
                if (argv[i] == 't') {
                    opt->t = 1;
                    opt->v = 1;
                }
            }
    }
    if (strcmp(argv, "--number-nonblank") == 0) {
        opt->b = 1;
    } else if (strcmp(argv, "--number") == 0) {
        opt->n = 1;
    } else if (strcmp(argv, "--squeeze-blank") == 0) {
        opt->s = 1;
    }
}

void opt_n(int i) {
    printf("%6d\t", i);
}

void opt_b(char *str, int *j) {
    if (strcmp(str, "\n") != 0) {
        printf("%6d\t", (*j)++);
    }
}

void print_file(options opt, char *str, ssize_t count_read) {
    for (ssize_t i = 0; i < count_read; i++) {
        if (opt.v == 1) {
            if (str[i] != '\n') {
                if ((str[i]  >= 0 && str[i] < 9) || (str[i]  >= 10 && (str[i] < 32))|| str[i] == 127) {
                    if (str[i] == 127) {
                        printf("^%c", str[i] - 64);
                    } else {
                        printf("^%c", str[i] + 64);
                    }
                    continue;;
                }
            }
        }
        if (opt.t == 1) {
            if (str[i] == '\t') {
                printf("^I");
                continue;;
            }
        }
        if (opt.e == 1) {
            if (str[i] == '\n') {
                {
                    printf("$");
                }
            }
        }
        printf("%c", str[i]);
    }
}
