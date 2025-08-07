# Simple Bash Utils

This project implements simplified versions of the classic Unix utilities **cat** and **grep** in C (C11 standard).

## Features

- **s21_cat**  
  Supports flags: `-b`, `-e`, `-n`, `-s`, `-t` (GNU-compatible)

- **s21_grep**  
  Supports flags: `-e`, `-i`, `-v`, `-c`, `-l`, `-n`  
  Bonus support for additional flags: `-h`, `-s`, `-f`, `-o` and their combinations

- Uses PCRE or POSIX regex libraries for pattern matching  
- Follows Google C Style Guide and POSIX.1-2017 standards  
- Built with Makefiles; executables are located in `src/cat/` and `src/grep/`

## Project Structure

```
src/
├── cat/       # Source, headers, Makefile, s21_cat executable
├── grep/      # Source, headers, Makefile, s21_grep executable
└── common/    # Shared modules reused by both utilities
```

## Build Instructions

Run `make` in the desired directory to build the corresponding utility:

```bash
cd src/cat && make     # Builds s21_cat
cd src/grep && make    # Builds s21_grep
```

## Usage Examples

```bash
./s21_cat -n file.txt
./s21_grep -i "pattern" file.txt
```

---

Feel free to customize the code and flags according to your needs.  
Contributions and suggestions are welcome!
