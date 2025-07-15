#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <regex.h>

typedef struct options {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;

    char *pattern;
} opt;

void parser(int argc, char * argv[], opt *options);
void reader(int argc, char *argv[], opt *options);


#endif 