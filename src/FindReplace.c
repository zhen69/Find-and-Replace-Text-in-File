#include "FindReplace.h"
extern char *optarg;

int searchLen;
char search_text[MAX_TEXT_LEN + 1], replace_text[MAX_TEXT_LEN + 1];

FILE *inputFile, *outputFile;

/*
checkFlags checks if a flag is entered by the user.
checkFlags = {s flag, r flag, l flag, w flag, dublicate flag}
*/
bool checkFlags[] = {false, false, false, false, false};

/*
argErrors indicates if an argument is missing/invalid for a specific flag.
argErrors = {s_flag_arg, r_flag_arg, l_flag_r}
*/
bool argErrors[] = {false, false, false, false};

/**
 * @brief 
 * This program would perform find and replace on the input file 
 * and print the results in the indicated output file.
 * 
 * @param argc
 *      Integer indicating the number of command line arguments.
 * @param argv
 *      Array containing the input command line arguments.
 * @return
 *      0 if program terminates successfully, other a non-negative error code.
 */
int main(int argc, char *argv[]){
    /*
    Requried Arguments:
    - Program name
    - s flag
    - s flag argument
    - r flag
    - r flag argument
    - input file
    - output file
    */
    if(argc < 7)
        return MISSING_ARGUMENT;
    
    /*
    start_end_lines indicates the range of the replacement process
    start_end_lines = {starting line num, ending line num}
    */
    int start_end_lines[] = {1, INT_MAX};

    scanArgs(argc, argv, start_end_lines);

    int error;

    searchLen = strlen(search_text);

    if((error = checkErrors(argv[argc - 2], argv[argc - 1]))) return error;
    
    /*
    mode = 0: replaces all search_text.

    mode = 1: replaces all texts with a prefix of search_text.

    mode = 2: replaces all texts with a suffix of search_text.
    */
    int mode = 0;
    if(search_text[searchLen - 1] == '*') mode = 1;
    else if(search_text[0] == '*') mode = 2;

    findReplace(mode, start_end_lines);
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}


/**
 * @brief
 *      Scans the command line arguments and calls the corresponding functions.
 * 
 * @param argc 
 *      Integer indicating the number of command line arguments.
 * @param argv 
 *      Array storing the command line arguments.
 * 
 * @param start_end_lines 
 *      Array containing the starting/ending line numbers.
 */
void scanArgs(const int argc, char* const* argv, int *start_end_lines){
    extern int opterr, optopt;
    int option;
    opterr = 0;

    while((option = getopt(argc, argv, "s:r:l:w")) != -1){
        switch(option){
            case 's': {
                srFlag(checkFlags, argErrors, true);
                break;
            }
            case 'r': {
                srFlag(checkFlags + 1, argErrors + 1, false);
                break;
            }
            case 'l': {
                lFlag(start_end_lines);
                break;
            }
            case 'w': {
                wFlag();
                break;
            }
        }
    }
}

/**
 * @brief
 *      Indicates a s/r flag is scanned or a duplicate flag have occurred.
 * 
 * @param srFlags
 *      A pointer that points a boolean value that indicates whether a s/r flag is scanned.
 * @param argError
 *      A pointer that points a boolean value that indicates wheteher a argument error is encountered.
 * @param s_flag 
 *      A boolean value indicating whether the function is dealing with a s flag.
 * 
 * @note
 *      It initializes search and replacement texts if no error is encountered.
 */
void srFlag(bool *srFlags, bool *argError, bool s_flag){
    if(*srFlags){
        /*
        If s_flag, *flags = checkFlags[0] where flags[4] = checkFlags[4] = boolean value of Duplicate Flag.
        
        Else, *flag = checkFlags[1] where flags[3] = *(checkFlags + 1 + 3) = checkFlags[4] = boolean value 
        of Duplicate Flag
        */
        if(s_flag) srFlags[4] = true; 
        else srFlags[3] = true;
        return;
    }

    if(optarg == NULL || optarg[0] == '-'){
        *argError = true;
        return;
    }

    if(s_flag) sprintf(search_text, "%s", optarg);
    else sprintf(replace_text, "%s", optarg);


    *srFlags = true;
}


/**
 * @brief 
 *      Indicates a l flag is scanned or a duplicate flag have occurred. 
 * 
 * @note
 *      If no duplicate, it initializes the starting/ending line numbers.
 * 
 * @param start_end_lines
 *      Array that contains the starting/ending line numbers.
 */
void lFlag(int *start_end_lines){
    if(checkFlags[2]){
        checkFlags[2] = true;
        return;
    }

    char *startLine, *endLine;
    int start, end;

    /*
    optarg for the l flag must not start with '-', must includes both starting and ending line numbers 
    where start <= end, and must contains a comma sperating the line numbers.
    */
    if(optarg == NULL || optarg[0] == '-' || strchr(optarg, ',') == NULL
        || !(startLine = strtok(optarg, ",")) || !(endLine = strtok(NULL, ","))
        || (start = strtol(startLine, NULL, 10)) > (end = strtol(endLine, NULL, 10))){

        argErrors[2] = true;
        return;
    }

    start_end_lines[0] = start;
    start_end_lines[1] = end;
    checkFlags[2] = true;
}


/**
 * @brief 
 *      Indicates a w flag is scanned or a duplicate flag have occurred.
 */
void wFlag(){
    if(checkFlags[3]) checkFlags[4] = true;
    else checkFlags[3] = true;
}


/**
 * @brief 
 *      Returns an error code based on order of precedence.
 * 
 * @param input_file 
 *      A string indicating the input file.
 * @param output_file 
 *      A string indicating the output file.
 * 
 * @return
 *      An error code if an error occurs, otherwise 0.
 */
int checkErrors(char *input_file, char *output_file){
    /*
    Error precedence from from high to low:
        1. DUPLICATE_ARGUMENT 
        2. INPUT_FILE_MISSING
        3. OUTPUT_FILE_UNWRITABLE
        4. S_ARGUMENT_MISSING
        5. R_ARGUMENT_MISSING
        7. L_ARGUMENT_INVALID
        8. WILDCARD_INVALID
     */
    if(checkFlags[4]) return DUPLICATE_ARGUMENT;

    obtainFile(input_file, output_file);

    if(!inputFile) return INPUT_FILE_MISSING;
    if(!outputFile) return OUTPUT_FILE_UNWRITABLE;
    if(!checkFlags[0] || argErrors[0]) return S_ARGUMENT_MISSING;
    if(!checkFlags[1] || argErrors[1]) return R_ARGUMENT_MISSING;
    if(argErrors[2]) return L_ARGUMENT_INVALID;
    if(checkFlags[3]) return checkSearchTextForW();
    return 0;
}


/**
 * Opens the files indicated by input and output.
 * 
 * @param input
 *      A string indicating the input file.
 * 
 * @param output
 *      A string indicating the output file.
 * 
 * @warning
 *      Must declare global variables inputFile and outputFile before using this fucntion.
 * 
 * @note
 *      When both inputFile and outputFile points to the same file, inputFile would be modified to point to 
 *      a temporary file that contains the same info as the orginal pointed file.
*/
void obtainFile(char *input, char *output){
    inputFile = fopen(input, "r");

    if(!strcmp(input, output)){
        FILE *tempFile = tmpfile();
        char line[MAX_LINE];

        while(fgets(line, MAX_LINE, inputFile)) fputs(line, tempFile);

        rewind(tempFile);
        inputFile = tempFile;
    }

    outputFile = fopen(output, "w");
}


/**
 * @brief
 *      Checks if the search text is appropriate for the wildcard search/replace to be performed.
 * 
 * @note
 *      Prefix search: search text must ends with a '*'
 * @note
 *      Suffix search: search text must starts with a '*'
 * 
 * @return
 *      WILDCARD_INVALID if search text is invalid, otherwise 0;
*/
int checkSearchTextForW(){
    bool doubleAsteriskError = (search_text[0] == '*' && search_text[searchLen - 1] == '*'),
            noAsteriskError = (search_text[0] != '*' && search_text[searchLen - 1] != '*');

    if(doubleAsteriskError || noAsteriskError) return WILDCARD_INVALID;
    return 0;
}

void findReplace(int mode, int *start_end_lines){
    char curr_line[MAX_LINE + 1];
    int lineNum = 1;
    while(fgets(curr_line, MAX_LINE, inputFile) != NULL){
        if(start_end_lines[0] <= lineNum && start_end_lines[1] >= lineNum){
            if(mode == 0) 
                replace(curr_line);
            else if(mode == 1){
                search_text[searchLen - 1] = '\0';
                prefixReplace(curr_line, search_text);
            }
            else
                suffixReplace(curr_line, search_text + 1);
        }
        else 
            fputs(curr_line, outputFile);
        lineNum++;
    }
}

void replace(char *curr_line){
    if(strcmp(search_text, replace_text) == 0){
        fputs(curr_line, outputFile);
        return;
    }

    char *word_found = strstr(curr_line, search_text);
    while(word_found){
        int indexOfWord = (word_found - curr_line), lineLen = strlen(curr_line);
        
        fprintf(outputFile, "%.*s%s", indexOfWord, curr_line, replace_text);

        if(*(word_found + searchLen) == '\0') return;
        
        getRemainingTexts(curr_line, word_found + searchLen, (lineLen - (searchLen + indexOfWord)) + 1);
        word_found = strstr(curr_line, search_text);
    }
    fputs(curr_line, outputFile);
}


void prefixReplace(char *curr_line, char *prefix){
    char *prefix_found = strstr(curr_line, prefix);
    while(prefix_found){
        int indexOfWord = prefix_found - curr_line, lineLen = strlen(curr_line), 
            endOfWordIndex = updateEndIndex(curr_line, lineLen, indexOfWord + searchLen - 1);

        if(indexOfWord == 0 || !isalnum(curr_line[indexOfWord - 1]))
            fprintf(outputFile, "%.*s%s", indexOfWord, curr_line, replace_text);
        else
            fprintf(outputFile, "%.*s", endOfWordIndex, curr_line);
        
        if(*(curr_line + endOfWordIndex) == '\0') return;
        
        getRemainingTexts(curr_line, curr_line + endOfWordIndex, (lineLen - endOfWordIndex) + 1);
        prefix_found = strstr(curr_line, prefix);
    }
    fputs(curr_line, outputFile);
}


void suffixReplace(char *curr_line, char *suffix){
    char *suffix_found = strstr(curr_line, suffix);
    while(suffix_found){
        int indexOfWord = suffix_found - curr_line, lineLen = strlen(curr_line),
            endOfWordIndex = updateEndIndex(curr_line, lineLen, indexOfWord + searchLen - 1);
        
        if(strncmp(suffix, curr_line + (endOfWordIndex - (searchLen - 1)), searchLen - 1) == 0){
            int startOfWordIndex = updateStartIndex(curr_line, indexOfWord - 1);
            fprintf(outputFile, "%.*s%s", startOfWordIndex + 1, curr_line, replace_text);
        }
        else fprintf(outputFile, "%.*s", endOfWordIndex, curr_line);

        if(*(curr_line + endOfWordIndex) == '\0') return;

        getRemainingTexts(curr_line, curr_line + endOfWordIndex, (lineLen - endOfWordIndex) + 1);
        suffix_found = strstr(curr_line, suffix);
    }
    fputs(curr_line, outputFile);
}


int updateEndIndex(char *line, int lineLen, int endIndex){
    while(endIndex < lineLen && isalnum(line[endIndex])) endIndex++;
    return endIndex;
}


int updateStartIndex(char *line, int startIndex){
    while(startIndex >= 0 && isalnum(line[startIndex])) startIndex--;
    return startIndex;
}


void getRemainingTexts(char *curr_line, char *startOfRemain, int sizeOfRemain){
    char remain[sizeOfRemain];
    strcpy(remain, startOfRemain);
    strcpy(curr_line, remain);
}