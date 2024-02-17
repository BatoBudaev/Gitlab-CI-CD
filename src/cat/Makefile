CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -c

TARGET1 = s21_cat

all: $(TARGET1) clean

$(TARGET1) : s21_cat.o
	$(CC) s21_cat.o -o $(TARGET1)

s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat.o

test:
	bash t_cat.sh

clang:
	clang-format -n -style=google *.c *.h

leak:
	valgrind --leak-check=full --leak-resolution=med ./s21_cat bytes.txt

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c

clean:
	rm -rf *.o

rebuild: all