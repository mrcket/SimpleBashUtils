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
                // Флаг -s (должен быть в начале)
                if (options->s) {
                    if (last_char == '\n' && current_char == '\n') {
                        line_beginning = 1;
                        last_char = current_char;
                        continue;
                    }
                    last_char = current_char;
                }
                
                // Нумерация строк
                if (line_beginning) {
                    if (options->b && current_char != '\n') {
                        printf("%6d\t", line_number++);
                    } else if (options->n && !options->b) {
                        printf("%6d\t", line_number++);
                    }
                    line_beginning = 0;
                }

                // 1. Обработка -v ДО других флагов
                if (options->v) {
                    if (current_char == 127) {
                        printf("^?");
                        continue;
                    }
                    else if (current_char < 32) {
                        if (current_char != '\t' && current_char != '\n') {
                            printf("^%c", current_char + 64);
                            continue;
                        }
                    }
                    else if (current_char >= 128) {
                        int c = current_char - 128;
                        if (c == 127) {
                            printf("M-^?");
                        }
                        else if (c < 32) {
                            printf("M-^%c", c + 64);
                        }
                        else { 
                            printf("M-%c", c);
                        }
                        continue;
                    }
                }

                // 2. Обработка -e
                if (options->e && current_char == '\n') {
                    printf("$");
                }

                // 3. Обработка -t/-T
                if (options->t && current_char == '\t') {
                    printf("DEBUG: TAB CHARACTER FOUND!\n");
                    printf("^I");
                    continue;  // Пропускаем основной вывод
                }

                // Основной вывод символа
                printf("%c", current_char);

                // Обновление состояния переноса строки
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