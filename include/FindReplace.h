#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

//A line of the input file could have at most MAX_LINE + 1 characters (including the null terminator)
#define MAX_LINE 200

//A search/replace text could have at most MAX_TEXT_LEN + 1 characters (including the null terminator)
#define MAX_TEXT_LEN 50

#define MISSING_ARGUMENT 1
#define INPUT_FILE_MISSING 2
#define OUTPUT_FILE_UNWRITABLE 3
#define S_ARGUMENT_MISSING 4
#define R_ARGUMENT_MISSING 5
#define L_ARGUMENT_INVALID 6
#define WILDCARD_INVALID 7
#define DUPLICATE_ARGUMENT 8

void getRemainingTexts(char *curr_line, char *startOfRemain, int sizeOfRemain);
void suffixReplace(char *curr_line, char *suffix);
void prefixReplace(char *curr_line, char *prefix);
int updateStartIndex(char *line, int startIndex);
int updateEndIndex(char *line, int lineLen, int endIndex);
void replace(char *curr_line);
void findReplace(int mode, int *start_end_lines);
void obtainFile(char *input, char *output);
int checkSearchTextForW();
int checkErrors(char *input_file, char *output_file);
void srFlag(bool *flags, bool *argError, bool s_flag);
void lFlag(int *start_end_lines);
void wFlag();
void scanArgs(const int argc, char* const* argv, int *start_end_lines);

