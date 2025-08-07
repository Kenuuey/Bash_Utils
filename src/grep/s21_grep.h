#ifndef SRC_CAT_S21_GREP_H_
#define SRC_CAT_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_01 "Grep: %s: No such file or directory\n"

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int countFiles;
  int numberLine;
  int countMatches;
} Options;

int ReadFlags(int argc, char **argv, Options *flags, char **template);
void PrintMainInfo(char *str, Options *Options, char *tmpl, char *name);
void setFlagF(Options *flags, char **template, char *optarg);
void setFlagE(Options *flags, char **template, char *optarg);
void printFlagO(regex_t my_regex, char *str, Options flags);
void Grep(char *name, Options flags, char *tmpl);
void printAuxInfo(Options flags, char *path);
void *increaseLengthStr(void *str, int size);
int createTemplate(char **str, char *optarg);
void setupflags(Options *flags, int argc);
void addTemplate(char **str, char *optarg);

#endif  // SRC_CAT_S21_GREP_H