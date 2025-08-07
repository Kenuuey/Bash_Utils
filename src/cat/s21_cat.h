#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

typedef struct {
    bool numberNonBlank; // -b --number-nonblank
    bool markEndl;      // -e -E
    bool numberAll;     // -n --number
    bool squeeze;       // -s --squeeze-blank
    bool tab;           // -t -T
    bool printNonPrintable; // -v
} Flags;

Flags CatReadFlags(int argc, char *argv[]);
void CatReadStream(int argc, char *argv[], Flags flags);
void CatReadFile(FILE *file, Flags flags);

#endif  // SRC_CAT_S21_CAT_H