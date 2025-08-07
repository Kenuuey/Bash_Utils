#include "s21_cat.h"

// tests, only running cat

int main(int argc, char *argv[]) {
  Flags flags = CatReadFlags(argc, argv);
  CatReadStream(argc, argv, flags);
  // CatReadFile(stdin, flags);
  return 0;
}

void CatReadStream(int argc, char *argv[], Flags flags) {
  if (argc == 1) {
    char c;
    while (fread(&c, 1, 1, stdin) > 0) {
      putchar(c);
    }
  } else {
    // iterates over the command-line arguments starting // from the second
    // argument (&argv[1]) to the end of the arguments (&argv[argc]
    for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
         ++filename) {
      if (**filename == '-') continue;

      // **** cat -***

      FILE *file = fopen(*filename, "rb");
      if (errno) {
        fprintf(stderr, "%s: ", argv[0]);
        perror(*filename);
        continue;
      }
      CatReadFile(file, flags);  // If the file opens successfully, the function
                                 // calls CatReadfile
      fclose(file);
    }
  }
}
void CatReadFile(FILE *file, Flags flags) {
  int c = 0;             // Stores the current character read from the file
  int last = '\n';       // Holds the last character read from the file
  int line_number = 0;   // Counts the line numbers for numbered lines
  bool squeeze = false;  // A flag indicating whether consecutive blank lines
                         // should be squeezed together

  while (fread(&c, 1, 1, file) > 0) {
    if (flags.tab && c == '\t') {  // t
      printf("%s", "^I");
      continue;
    }

    if (flags.markEndl && c == '\n') {  // e
      printf("%s", "$");
      // continue;
    }

    if (last == '\n') {
      if (flags.squeeze && c == '\n') {  // s
        if (squeeze) continue;
        squeeze = true;
      } else {
        squeeze = false;
      }

      if (flags.numberNonBlank && c != '\n') {  // b n
        printf("%6i\t", ++line_number);
      } else if (flags.numberAll) {  // n
        printf("%6i\t", ++line_number);
      }
    }

    putchar(c);
    last = c;
  }
  // if (flags.numberAll) {
  //     printf("\n");
  // }
}
Flags CatReadFlags(int argc, char *argv[]) {
  // parsing long-options using getopt_long function
  // struct option {const char *name; int has_arg; int *flag; int val;};
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};
  int current_flag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
  Flags flags = {false, false, false, false, false, false};
  for (; current_flag != -1;
       current_flag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) {
    switch (current_flag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.markEndl = true;
        break;
      case 'v':
        flags.printNonPrintable = true;
        break;
      case 'E':
        flags.markEndl = true;
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.printNonPrintable = true;
      case 'T':
        flags.tab = true;
    }
  }
  return flags;  // contains the state of all parsed options
}