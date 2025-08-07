CC=gcc
FLAGS=-Wall -Werror -Wextra -g -std=c11 -pedantic

all: build

build: s21_cat

s21_cat: s21_cat.o
	$(CC) $(FLAGS) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) s21_cat.c -c

test: s21_cat
	python3 ./TEST.py

check:
	clang-format -style=Google -n *.c

clang:
	clang-format -style=Google -i *.c

clean:
	rm -rf s21_cat.o s21_cat

rebuild: clean all