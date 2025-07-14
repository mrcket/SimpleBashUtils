#include "s21_grep.h"

int main(int argc, char ** argv)
{
    opt options = {0};
    parser(argc, argv, &options);
    reader(argc, argv, &options);

    return 0;
}

void parser(int argc, char * argv[], opt *options){
    int option;

    while((option = getopt(argc, argv,"e:ivcln")) != -1){
        switch(option){
            case 'e':
                options->e = 1;
                options->pattern = optarg;
                break;
            case 'i':
                options->i = 1;
                break;
            case 'n':
                options->n = 1;
                break;
            case 'v':
                options->v = 1;
                break;
            case 'c':
                options->c = 1;
                break;
            case 'l':
                options->l = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-e pattern] [-ivcln] [file...]\n", argv[0]);
                exit(1);
        }
    }

    if(!options->e && optind < argc){
        options->pattern = argv[optind++];

    }
}

void reader(int argc, char *argv[], opt *options) {

    regex_t regex;
    int flags = REG_EXTENDED | (options->i ? REG_ICASE : 0);
    int error = regcomp(&regex, options->pattern, flags);
        if (error){
            perror("error");

        }
    for (int i = optind; i < argc; i++) {
        int line_number= 0;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        FILE *file = fopen(argv[i], "r");
        if (file) {
            while ((read = getline(&line, &len, file)) != -1) {
                int result = regexec(&regex, line, 0, NULL, 0);
                line_number++;
                if(options->v && result != 0) printf("%s", line);
                if(options->l && result == 0) break;
                if(options->i && result == 0){
                   printf("%s", line); 
                }

                if(options->e && result ==0 ) printf("%s", line);

                if(options->n && result ==0){  
                    printf("%d %s", line_number, line);
                }
            }
            if(options->c){
                printf("%d", line_number);
            }
            if(options->l){
                printf("%s", argv[i]);
            }
            fclose(file);
        } else {
            fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[i]);
        }
    }
}