#include "s21_grep.h"

int main(int argc, char **argv) {
  struct flags values;
  char *patterns[BUFFER_SIZE] = {0};
  char *filePatternsIndex[BUFFER_SIZE] = {0};
  int patternsCount = 0;

  patternsCount = parse(argc, argv, &values, patterns, filePatternsIndex);
  if (patternsCount != -1) {
    doOptions(argc, argv, &values, patterns, patternsCount);
  }

  for (int i = 0; i < patternsCount; i++) {
    if (filePatternsIndex[i] != NULL) free(patterns[i]);
  }

  return 0;
}

int parse(int argc, char **argv, struct flags *values, char *patterns[],
          char *filePatternsIndex[]) {
  int result;
  int patternsCount = 0;

  values->e = false;
  values->i = false;
  values->v = false;
  values->c = false;
  values->l = false;
  values->n = false;
  values->h = false;
  values->s = false;
  values->f = false;
  values->o = false;

  while ((result = getopt_long(argc, argv, "ivclnhsoe:f:", NULL, NULL)) != -1) {
    switch (result) {
      case 'e':
        values->e = true;
        patterns[patternsCount] = optarg;
        patternsCount++;
        break;
      case 'i':
        values->i = true;
        break;
      case 'v':
        values->v = true;
        break;
      case 'c':
        values->c = true;
        break;
      case 'l':
        values->l = true;
        break;
      case 'n':
        values->n = true;
        break;
      case 'h':
        values->h = true;
        break;
      case 's':
        values->s = true;
        break;
      case 'f':
        values->f = true;
        getPatternsFromFile(optarg, patterns, &patternsCount,
                            filePatternsIndex);
        break;
      case 'o':
        values->o = true;
        break;
      default:
        break;
    }
  }

  return patternsCount;
}

void getPatternsFromFile(char *fileName, char *patterns[], int *patternsCount,
                         char *filePatternsIndex[]) {
  FILE *file = NULL;
  file = fopen(fileName, "r");
  if (file != NULL) {
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
      if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
      }
      patterns[(*patternsCount)] = doStrdup(buffer);
      filePatternsIndex[(*patternsCount)] = "1";
      (*patternsCount)++;
    }
    fclose(file);
  } else {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", fileName);
    *patternsCount = -1;
  }
}

char *doStrdup(char *src) {
  char *str;
  char *p;
  int len = 0;

  while (src[len]) len++;
  str = malloc(len + 1);
  p = str;
  while (*src) *p++ = *src++;
  *p = '\0';
  return str;
}

void doOptions(int argc, char **argv, struct flags *values, char *patterns[],
               int patternsCount) {
  FILE *file = NULL;
  char searchLine[BUFFER_SIZE] = {0};
  int filesCount = 0;
  int arrayVal = 0;

  if (values->e == true || values->f == true) {
    filesCount = argc - optind;

    if (patternsCount > 0) {
      for (int i = 0; i < patternsCount; i++) {
        strcat(searchLine, patterns[i]);
        if (i + 1 < patternsCount) {
          strcat(searchLine, "|");
        }
      }
    } else {
      filesCount = 0;
    }
  } else {
    filesCount = argc - optind - 1;
    arrayVal = 1;
    strcat(searchLine, argv[argc - filesCount - arrayVal]);
  }

  for (int i = 0; i < filesCount; i++) {
    file = fopen(argv[optind + i + arrayVal], "r");
    if (file != NULL) {
      doGrep(file, values, searchLine, argv[optind + i + arrayVal], filesCount);
      fclose(file);
    } else {
      if (values->s == false) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                argv[optind + i + arrayVal]);
      }
    }
  }
}

void doGrep(FILE *file, struct flags *values, char searchLine[], char *fileName,
            int filesCount) {
  char buffer[BUFFER_SIZE];
  char prefix[100] = "";
  char linesNumberChar[100] = "";
  char matchString[100];
  regex_t regexStruct;
  regmatch_t pMatch;
  int regexFlag = 0;
  int linesNumber = 0;
  int linesCount = 0;
  int stopSearch = 0;
  int compare = 0;

  if (filesCount > 1 && values->h == false) {
    strcat(prefix, fileName);
    strcat(prefix, ":");
  }

  if (values->v == true) {
    compare = REG_NOMATCH;
  }

  if (values->i == true) {
    regexFlag = REG_ICASE;
  }

  if (values->e == true || values->f == true) {
    if (values->i == true) {
      regexFlag = REG_ICASE | REG_EXTENDED;
    } else {
      regexFlag = REG_EXTENDED;
    }
  }

  regcomp(&regexStruct, searchLine, regexFlag);

  while (fgets(buffer, BUFFER_SIZE, file) && stopSearch != 1) {
    int lineLength = strlen(buffer);
    int regexValue;

    if (buffer[lineLength - 1] == '\n') {
      buffer[lineLength - 1] = '\0';
    }

    linesNumber++;

    regexValue = regexec(&regexStruct, buffer, 1, &pMatch, 0);

    if (regexValue == compare) {
      linesCount++;

      if (values->c == false) {
        if (values->l == true) {
          printf("%s\n", fileName);
          stopSearch = 1;
        } else {
          if (values->n == true) {
            sprintf(linesNumberChar, "%d:", linesNumber);
          }

          if (values->o == true) {
            char *ptr = buffer;
            int flag = 0;
            while (!regexValue && flag != 1) {
              if (pMatch.rm_eo == pMatch.rm_so) {
                flag = 1;
              }
              if (flag == 0) {
                memset(matchString, '\0', 100);
                strncpy(matchString, &ptr[pMatch.rm_so],
                        pMatch.rm_eo - pMatch.rm_so);
                ptr += pMatch.rm_eo;
                printf("%s%s%s\n", prefix, linesNumberChar, matchString);
                regexValue = regexec(&regexStruct, ptr, 1, &pMatch, 0);
              }
            }
          } else {
            printf("%s%s%s\n", prefix, linesNumberChar, buffer);
          }
        }
      }
    }
  }

  regfree(&regexStruct);

  if (values->c == true) {
    if (values->l == false) {
      printf("%s%d\n", prefix, linesCount);
    }
  }
}
