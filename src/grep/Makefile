CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

all: s21_grep

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clean:
	rm -rf s21_grep

test: s21_grep
	chmod +x test_grep.sh
	./test_grep.sh
.PHONY: all clean test