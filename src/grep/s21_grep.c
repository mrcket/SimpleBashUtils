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
    if(!options->e){
        if (optind < argc){
            options->pattern = argv[optind++];
    }
    else {
        fprintf(stderr, "grep: pattern missing\n");
        exit(1);
    }
    }
}

void process_file(const char *filename, opt *options, regex_t *regex, int multi) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 0;
    int match_count = 0;
    int print_filename = (multi && !options->l && !options->c) ? 1 : 0;
    while ((read = getline(&line, &len, file)) != -1) {
        line_number++;
        int result = regexec(regex, line, 0, NULL, 0); 
        int match = (result == 0 && !options->v) || (result != 0 && options->v);
        if (match) {
            match_count++;
            if (options->l) {
                break;
            }
            if (!options->c) {
                if (print_filename) printf("%s:", filename);
                if (options->n) printf("%d:", line_number);
                fputs(line, stdout);
            }
        }
    }
    if (options->c) {
        if (multi) printf("%s:", filename);
        printf("%d\n", match_count);
    }
    if (options->l && match_count > 0) {
        printf("%s\n", filename);
    }
    free(line);
    fclose(file);
}

void reader(int argc, char *argv[], opt *options) {
    regex_t regex;
    int flags = REG_EXTENDED | (options->i ? REG_ICASE : 0); 
    if (regcomp(&regex, options->pattern, flags) != 0) {
        fprintf(stderr, "grep: invalid pattern\n");
        if (options->pattern) free(options->pattern);
        exit(1);
    }
    int multi = (argc - optind > 1) ? 1 : 0;
    for (int i = optind; i < argc; i++) {
        process_file(argv[i], options, &regex, multi);
    }
    regfree(&regex);
}

