CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf s21_cat

test: s21_cat
	chmod +x test_cat.sh
	./test_cat.sh
.PHONY: all clean test