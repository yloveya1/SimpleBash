

#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>



typedef struct {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} options;

void init_options(options *opt, char *argv);
void opt_n(int i);
void opt_b(char *str, int *j);
void opt_e(char **str, size_t *n);
void opt_s(char *str);
void opt_t(char **str, size_t *n, ssize_t count);
void opt_v(char **str, size_t *n, ssize_t count);
void print_file(options opt, char *str, ssize_t count_read);

#endif  // SRC_CAT_S21_CAT_H_

