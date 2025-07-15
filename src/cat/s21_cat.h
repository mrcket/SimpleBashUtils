#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

const char* shortoptions = "+benstvTE";

typedef struct options {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} opt;

const struct option long_options[] = {
    {"number-nonblank", no_argument, 0, 'b'},
    {"number",          no_argument, 0, 'n'},
    {"squeeze-blank",   no_argument, 0, 's'},
    {0, 0, 0, 0}
};

void parser(int argc, char * argv[], opt *options);
void reader(int argc, char *argv[], opt *options);


#endif 