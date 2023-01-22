#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#define _POSIX_C_SOURCE 200809L
#include <regex.h>  // include print
#include <stdio.h>  // int
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
struct list {
    char * data;
    struct list *ptr;
    int file;
};


typedef struct {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
} options;

int init_options(options *opt, int count, char **argv, struct list* current, int *file_index, size_t *end);
void add_pattern_file(struct list **current, char *file_name, size_t *end);

void find_word(char *str, char *find, options opt, int * check);
void print(options opt, char *argv, size_t count_of_files, char *str, size_t number);
void check_c_and_l(options opt, char *str, size_t fcount, size_t lcount, char *file_name, char *pattern);
int init_flags(char symb, options *opt);
void check_v(options opt, int *check, size_t fc, size_t *lc, int end, char *file, char *str,
    int num, char *patt);
void flag_o(char *str, struct list *p_list, options opt, size_t num, size_t fcount, char *filename);
void check_o(options *opt, struct list *p);
#endif  // SRC_GREP_S21_GREP_H_

