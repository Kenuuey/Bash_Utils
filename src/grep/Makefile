CC=gcc
FLAGS=-Wall -Werror -Wextra -g -std=c11 -pedantic

21_grep: s21_grep.o
	$(CC) $(FLAGS) s21_grep.o -o s21_grep

21_grep.o: s21_grep.c
	$(CC) s21_grep.c -c

test: s21_grep
	python3 ./TEST.py

check:
	clang-format -style=Google -n *.c

clang:
	clang-format -style=Google -i *.c

clean:
	rm -rf *.o s21_grep TEST-FILE-0.testing TEST-FILE-1.testing

rebuild: clean all