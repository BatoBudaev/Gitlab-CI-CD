#include "s21_cat.h"

int main(int argc, char **argv) {
  struct flags values;
  int optionsCount;

  optionsCount = parse(argc, argv, &values);

  if (!(argc >= 3 && optionsCount == 0)) {
    doOptions(argc, argv, &values);
  }

  return 0;
}

int parse(int argc, char **argv, struct flags *values) {
  int result;
  int optionsCount = 0;

  values->b = false;
  values->e = false;
  values->n = false;
  values->s = false;
  values->t = false;
  values->v = false;
  values->E = false;
  values->T = false;

  static struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((result = getopt_long(argc, argv, "benstvET", long_options, NULL)) !=
         -1) {
    switch (result) {
      case 'b':
        values->b = true;
        optionsCount++;
        break;
      case 'e':
        values->e = true;
        values->v = true;
        optionsCount++;
        break;
      case 'n':
        values->n = true;
        optionsCount++;
        break;
      case 's':
        values->s = true;
        optionsCount++;
        break;
      case 't':
        values->t = true;
        optionsCount++;
        values->v = true;
        break;
      case 'v':
        values->v = true;
        optionsCount++;
        break;
      case 'E':
        values->e = true;
        optionsCount++;
        break;
      case 'T':
        values->t = true;
        optionsCount++;
        break;
      default:
        break;
    }
  }

  return optionsCount;
}
void doOptions(int argc, char **argv, struct flags *values) {
  FILE *file = NULL;

  file = fopen(argv[argc - 1], "r");
  if (file != NULL) {
    doCat(file, values);
    fclose(file);
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[argc - 1]);
  }
}

void doCat(FILE *file, struct flags *values) {
  char ch;
  int lineNumber = 1;
  char prevPreviousChar = {0};
  char previousChar = '\n';
  bool sFlag = false;
  while ((ch = (char)getc(file)) != EOF) {
    if (values->s == true) {
      if (prevPreviousChar == '\n' && previousChar == '\n' && ch == '\n') {
        sFlag = true;
      }
    }

    if (sFlag == false) {
      if (values->b == true) {
        if (previousChar == '\n' && ch != '\n') {
          printf("%6d\t", lineNumber++);
        }
      } else if (values->n == true) {
        if (previousChar == '\n') {
          printf("%6d\t", lineNumber++);
        }
      }

      if (values->e == true) {
        if (ch == '\n') {
          printf("$");
        }
      }

      if (values->t == true) {
        if (ch == '\t') {
          printf("%c", '^');
          ch += 64;
        }
      }

      if (values->v == true) {
        if ((ch >= 0) && (ch <= 31) && (ch != 9) && (ch != 10)) {
          printf("%c", '^');
          ch += 64;
        } else if (ch == 127) {
          printf("%c", '^');
          ch = 63;
        }
      }

      printf("%c", ch);
      prevPreviousChar = previousChar;
      previousChar = ch;
    }

    sFlag = false;
  }
}
