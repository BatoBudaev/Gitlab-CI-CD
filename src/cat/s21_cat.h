//
// Created by Nebomoe on 12.11.2022.
//

#ifndef C3_SIMPLEBASHUTILS_0_SRC_CAT_S21_CAT_H_
#define C3_SIMPLEBASHUTILS_0_SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>

typedef enum { false, true } bool;

struct flags {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
  bool E;
  bool T;
};

int parse(int argc, char **argv, struct flags *values);
void doOptions(int argc, char **argv, struct flags *values);
void doCat(FILE *file, struct flags *values);

#endif  // C3_SIMPLEBASHUTILS_0_SRC_CAT_S21_CAT_H_
