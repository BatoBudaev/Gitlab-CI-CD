//
// Created by Nebomoe on 12.11.2022.
//

#ifndef C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_
#define C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_
#define BUFFER_SIZE 4096

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false, true } bool;

struct flags {
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
};

int parse(int argc, char **argv, struct flags *values, char *patterns[],
          char *filePatternsIndex[]);
void doOptions(int argc, char **argv, struct flags *values, char *patterns[],
               int patternsCount);
void doGrep(FILE *file, struct flags *values, char searchLine[], char *fileName,
            int filesCount);
void getPatternsFromFile(char *fileName, char *patterns[], int *patternsCount,
                         char *filePatternsIndex[]);
char *doStrdup(char *src);

#endif  // C3_SIMPLEBASHUTILS_0_SRC_GREP_S21_GREP_H_
