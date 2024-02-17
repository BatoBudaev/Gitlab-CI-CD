CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -c

TARGET1 = s21_grep

all: $(TARGET1) clean

$(TARGET1) : s21_grep.o
	$(CC) s21_grep.o -o $(TARGET1)

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep.o

test:
	bash grep_test.sh

clang:
	clang-format -n -style=google *.c *.h

leak:
	valgrind --leak-check=full --leak-resolution=med ./s21_grep hello test.txt

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_grep.c

clean:
	rm -rf *.o

rebuild: all
