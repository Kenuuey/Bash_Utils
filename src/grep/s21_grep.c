#include "./s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 1) {       // checks if there are command-line arguments
    char *temp = NULL;  // initializes a temp string pointer
    Options flags = {
        0};  // Options structure named flags to store various flags

    // Parsing command-line arguments and populate the flags structure
    // accordingly The temp string is used to store templates for pattern
    // matching
    for (int x = ReadFlags(argc, argv, &flags, &temp); x < argc; x += 1)
      // After parsing the flags, it iterates over the remaining command-line
      // arguments, treating each as a file path to be searched
      // For each file, it calls the Grep function to perform the search.
      Grep(argv[x], flags, temp);
    if (temp != NULL)
      free(temp);  // if temp was allocated, it frees the memory to prevent
                   // memory leaks
  }
  return 0;
}

int ReadFlags(int argc, char **argv, Options *flags, char **template) {
  // using getopt to parse command-line arguments.
  for (int sym = 0; (sym = getopt(argc, argv, "e:ivclnhsf:o")) != (-1);) {
    switch (sym) {
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
        // If neither -e nor -f is set, it creates a default
        // template from the next positional argument (argv[optind]).
      case 'e':
        setFlagE(flags, template, optarg);
        break;
      case 'f':
        setFlagF(flags, template, optarg);
        break;
    }
  }
  if ((flags->e || flags->f) == 0) {
    createTemplate(template, argv[optind]);
    optind += 1;
  }
  setupflags(flags, argc);
  return optind;
}

// Grep opens each specified file and reads it character by character.
// For each line, it allocates memory for a line buffer and reads characters
// into this buffer until it encounters a newline character (\n) or reaches the
// end of the file (EOF).
void Grep(char *path, Options flags, char *template) {
  FILE *file = fopen(path, "r");
  if (file != NULL) {
    for (char sym = '0'; (sym = getc(file)) != EOF;) {
      char *line = calloc(256, 1);
      flags.numberLine += 1;
      int length = 0;

      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0) line = increaseLengthStr(line, length + 256);
      }
      // It then passes the line buffer to PrintMainInfo to perform
      // the actual pattern matching using regular expressions.
      PrintMainInfo(line, &flags, template, path);
      free(line);  // After processing a line, it frees the line buffer to
                   // manage memory efficiently.
    }
    printAuxInfo(flags, path);  // At the end of the file, it prints auxiliary
                                // information (if requested)
    fclose(file);               // and closes the file.
  } else if (flags.s == 0) {
    fprintf(stderr, ERROR_01, path);
  }
}

// The -f option allows users to specify a file from which to read patterns for
// searching
void setFlagF(Options *flags, char **template, char *optarg) {
  FILE *file = fopen(
      optarg, "r");  // It opens the file whose name is given by optarg (the
                     // argument associated with the -f option) for reading.
  if (file != NULL) {
    for (char sym = '0'; (sym = getc(file)) !=
                         EOF;) {  // reads the file character by character.
      int length = 0;
      char *line =
          calloc(256, 1);  // For each character, it appends the character to a
                           // dynamically allocated line buffer, ensuring that
                           // the buffer is large enough to hold the characters
      // Once a newline character ('\n') or the end of the file (EOF)
      // is reached, it checks whether either the -e or -f flag is set.
      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0)
          line = (char *)increaseLengthStr(line, length + 256);
      }
      // If not, it treats the contents of the line buffer as a new pattern and
      // stores it in the flags->f field of the Options structure using
      // createTemplate. If either flag is set, it adds the pattern to the
      // existing template using addTemplate.
      if (!(flags->e || flags->f))
        flags->f = createTemplate(template, line);
      else
        addTemplate(template, line);
      free(line);
    }
    fclose(file);
  } else if (flags->s == 0) {
    fprintf(stderr, ERROR_01, optarg);
  }
}

// setFlagE checks whether either the -e or -f flag is already set. If not, it
// treats the argument following -e as a new pattern and stores it in the
// flags->e field of the Options structure using createTemplate. If either flag
// is set, it adds the pattern to the existing template using addTemplate.
void setFlagE(Options *flags, char **template, char *optarg) {
  if (!(flags->e || flags->f))
    flags->e = createTemplate(template, optarg);
  else
    addTemplate(template, optarg);
}

// This function is called when the -e option is encountered in the command-line
// arguments. The -e option allows users to specify a pattern directly in the
// command line.
void printAuxInfo(Options flags, char *path) {
  if (flags.c) {    // // If the -c flag is set, it prints the number of matches
                    // found in the last file processed.
    if (flags.l) {  // If the -l flag is also set, it prints the filename
                    // followed by :1 if there were matches,
                    // // or just 1 otherwise. If the -l flag is not set, it
                    // prints the number of matches found in the last file.

      flags.countFiles > 1 ? printf("%s:1\n", path) : printf("1\n");
    } else {
      if (flags.countFiles > 1) printf("%s:", path);
      printf("%i\n", flags.countMatches);
    }
  }
  // If the -l flag is set and there were matches, it prints the
  // name of the file where matches were found.
  if (flags.l && flags.countMatches) printf("%s\n", path);
}

// PrintMainInfo compiles a regular expression from the template and executes it
// against the line. Depending on the flags set, it may count matches,
// print matched lines, or modify the output format.
void PrintMainInfo(char *line, Options *flags, char *template, char *path) {
  regex_t regex;
  if (regcomp(&regex, template, flags->i ? REG_ICASE : REG_EXTENDED) == 0) {
    if (regexec(&regex, line, 0, NULL, 0) == flags->v) {
      flags->countMatches += 1;
      if ((flags->c || flags->l) == 0) {
        if (flags->countFiles > 1 && !(flags->h)) printf("%s:", path);
        if (flags->n) printf("%i:", flags->numberLine);
        if (!flags->o)
          printf("%s\n", line);
        else
          printFlagO(regex, line, *flags);
      }
    }
    regfree(&regex);
  }
}

// The printFlagO to handle the -o option which instructs the program to print
// only the matching parts of lines
void printFlagO(regex_t regex, char *line, Options flags) {
  // Loop uses regexec to repeatedly test the regular expression against the
  // input line. This loop continues as long as there are matches.
  while (regexec(&regex, line, 0, NULL, 0) ==
         flags.v) {  // The condition regexec(&regex, line, 0, NULL, 0) ==
                     // flags.v checks if the regular
    // expression matches the line according to the flags (inverted match if
    // flags.v is true).
    char *aux = (char *)calloc(
        strlen(line) + 1,
        1);  // Auxiliary buffer (char *aux) is allocated with space for the
             // original line plus a null terminator.
    strcpy(
        aux,
        line);  // The line is copied into this buffer using strcpy(aux, line).
    int end = strlen(line);
    // A nested loop searches backward from the end of the line for
    // the beginning of the first match of the regular expression
    while (regexec(&regex, aux, 0, NULL, 0) == flags.v) {
      end--;  // This is done by repeatedly removing the last character from aux
      aux[strlen(aux) - 1] = 0;
    }
    aux[strlen(aux)] =
        line[strlen(aux)];  // and checking if the modified string still matches
                            // the regular expression
    int start = 0;
    //  The first match is removed from aux by shifting all characters after
    // the start of the match one position to the left.
    // This effectively removes the matched substring from aux.
    while (regexec(&regex, aux, 0, NULL, 0) == flags.v && strlen(aux) > 0) {
      start++;
      int j = 0;
      while (aux[j] != 0) {
        aux[j] = aux[j + 1];
        j++;
      }
    }
    start--;              // The characters in the original line that correspond
    int i = strlen(aux);  // to the removed match are shifted to fill the gap.
    while (i !=
           0) {  // This is done by copying characters from positions beyond
      aux[i] = aux[i - 1];  // the end of the match to their new positions.
      i--;
    }
    aux[0] = line[start];
    printf("%s\n", aux);  // The modified line, now missing the first match, is
                          // printed to the console.
    free(aux);  // After printing, the auxiliary buffer is freed to avoid memory
                // leaks.
    i = start + 1;
    while (line[i] !=
           0) {  // The outer loop ensures that this process repeats for all
      line[i - start - 1] = line[i];  // matches found in the line, effectively
      i++;
    }
    line[i - start - 1] = 0;  // isolating and printing each match separately.
  }
}

void setupflags(Options *flags, int argc) {
  if (flags->o && (flags->l || flags->v || flags->c)) flags->o = 0;
  flags->countFiles = argc - optind;
}

// Functions like createTemplate, addTemplate, and increaseLengthStr are used
// to manage the template string. Templates can be added through the -e and -f
// options, and they are concatenated with a pipe symbol (|) to allow for
// complex pattern matching.
int createTemplate(char **str, char *optarg) {
  *str = calloc(strlen(optarg) + 1, 1);
  if (*str) strcpy(*str, optarg);
  return str ? 1 : 0;
}

void addTemplate(char **str, char *optarg) {
  *str = increaseLengthStr(*str, strlen(*str) + strlen(optarg) + 2);
  if (*str) strcat(strcat(*str, "|"), optarg);
}

// increaseLengthStr dynamically increases the size of a string
// to accommodate new content, using realloc.
void *increaseLengthStr(void *str, int size) {
  char *aux = realloc(str, size);
  return aux;
}