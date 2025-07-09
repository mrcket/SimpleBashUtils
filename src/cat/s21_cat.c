#include "s21_cat.h"

int main(int argc, char ** argv)
{
    opt options = {0};
    parser(argc, argv, &options);
    reader(argc, argv, &options);

    return 0;
}

void parser(int argc, char * argv[], opt *options){
    int option;
    int option_index = -1;

    while((option = getopt_long(argc, argv, shortoptions, long_options, &option_index)) != -1){
        switch(option){
            case 'b':
                options->b=1;
                break;
            case 'e':
                options->e = 1;
                options->v = 1;
                break;
            case 'E':
                options->e = 1;
                break;
            case 'n':
                options->n = 1;
                break;
            case 's':
                options->s = 1;
                break;
            case 't':
                options->t = 1;
                options->v = 1;
                break;
            case 'T':
                options->t = 1;
                break;
            case 'v': 
                options->v = 1;
                break;
            default:
                fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
                exit(1);
        }

        option_index = -1;
    }
}

void reader(int argc, char *argv[], opt *options) {
    for (int i = optind; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file) {
            int current_char;
            int line_number = 1;
            int line_beginning = 1;
            int last_char = 0;

            while ((current_char = fgetc(file)) != EOF) {
                    if (line_beginning) {
                        if (options->b && current_char != '\n') {
                            printf("%6d\t", line_number++);
                        } else if (options->n && !options->b) {
                            printf("%6d\t", line_number++);
                        }
                        line_beginning = 0;
                    }

                    if (options->s) {
                        if (last_char == '\n' && current_char == '\n') {
                            line_beginning = 1;
                            continue;
                        }
                        last_char = current_char;
                    }

                    if (options->e && current_char == '\n') {
                        printf("$");
                    }

                    if (options->t && current_char == '\t') {
                        printf("^I");
                        continue;
                    }

                    
                    printf("%c",current_char);

                    if (current_char == '\n') {
                        line_beginning = 1;
                    }
            }
            fclose(file);
        } else {
            fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[i]);
        }
    }
}