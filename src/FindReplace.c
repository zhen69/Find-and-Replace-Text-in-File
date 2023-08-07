/**
 * @file FindReplace.c
 * 
 * @brief 
 *      Implemented a program that reads command line arguments, performs find/replace on the input file,
 *      and prints the results in the output file.
 * 
 * @author Zhen Wei Liao
 */
#include "FindReplace.h"

extern char *optarg;
int searchLen;

char search_text[MAX_TEXT_LEN + 1], replace_text[MAX_TEXT_LEN + 1];

FILE *inputFile, *outputFile;

/*
checkFlags checks if the user enters a specific flag.
checkFlags = {s flag, r flag, l flag, w flag, dublicate flag}
*/
bool checkFlags[] = {false, false, false, false, false};

/*
argErrors indicates if an argument is missing/invalid for a specific flag.
argErrors = {s_flag_arg, r_flag_arg, l_flag_r}
*/
bool argErrors[] = {false, false, false, false};


int main(int argc, char *argv[]){
    /*
    Required Arguments:
    - Program name
    - s flag
    - s flag argument
    - r flag
    - r flag argument
    - input file
    - output file
    */
    if(argc < 7) return MISSING_ARGUMENT;
    
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

    mode >= 2: replaces all texts with a suffix of search_text.
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
 * 
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
 *      Indicates a s/r flag is scanned or a duplicate flag has occurred.
 * 
 * @param srFlags
 *      Pointer that points a boolean value that indicates whether a s/r flag is scanned.
 * 
 * @param argError
 *      Pointer that points to a boolean value that indicates whether an argument error is encountered.
 * 
 * @param s_flag 
 *      Boolean value indicating whether the function is dealing with a s flag.
 * 
 * @note
 *      Function initializes search and replacement texts if no error is encountered.
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

    if(!optarg || optarg[0] == '-'){
        *argError = true;
        return;
    }

    if(s_flag) sprintf(search_text, "%s", optarg);
    else sprintf(replace_text, "%s", optarg);


    *srFlags = true;
}


/**
 * @brief 
 *      Indicates a l flag is scanned or a duplicate flag has occurred. 
 * 
 * @note
 *      If there is no duplicate, it initializes the starting/ending line numbers.
 * 
 * @param start_end_lines
 *      Array containing the starting/ending line numbers.
 */
void lFlag(int *start_end_lines){
    if(checkFlags[2]){
        checkFlags[2] = true;
        return;
    }

    char *startLine, *endLine;
    int start, end;

    /*
    optarg for the l flag must not start with '-' and must indicate both starting and ending line numbers (separated by a comma)
    where start <= end.
    */
    if(!optarg || optarg[0] == '-' || !strchr(optarg, ',')
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
 *      Indicates a w flag is scanned or a duplicate flag has occurred.
 */
void wFlag(){
    if(checkFlags[3]) checkFlags[4] = true;
    else checkFlags[3] = true;
}


/**
 * @brief 
 *      Returns an error code based on the order of precedence.
 * 
 * @param input_file 
 *      String indicating the input file.
 * 
 * @param output_file 
 *      String indicating the output file.
 * 
 * @return
 *      Error code if an error is encountered, otherwise 0.
 */
int checkErrors(char *input_file, char *output_file){
    /*
    Error precedence from high to low:
        1. DUPLICATE_ARGUMENT 
        2. INPUT_FILE_MISSING
        3. OUTPUT_FILE_UNWRITABLE
        4. S_ARGUMENT_MISSING
        5. R_ARGUMENT_MISSING
        7. L_ARGUMENT_INVALID
        8. WILDCARD_INVALID
     */
    if(checkFlags[4]) return DUPLICATE_ARGUMENT;

    obtainFiles(input_file, output_file);

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
 *      String indicating the input file.
 * 
 * @param output
 *      String indicating the output file.
 * 
 * @note
 *      When both inputFile and outputFile point to the same file, inputFile would be modified to point to 
 *      a temporary file that contains the same info as the original pointed file.
*/
void obtainFiles(char *input, char *output){
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
 *      Checks if the search text is appropriate for the prefix/suffix find and replace function 
 *      to be performed.
 * 
 * @note
 *      Prefix search: search text must end with a '*'
 * @note
 *      Suffix search: search text must start with a '*'
 * 
 * @return
 *      WILDCARD_INVALID if search text is invalid, otherwise 0;
*/
int checkSearchTextForW(){
    //search_text like *apple* or apple are not allowed when a w flag is presented
    bool doubleAsteriskError = (search_text[0] == '*' && search_text[searchLen - 1] == '*'),
            noAsteriskError = (search_text[0] != '*' && search_text[searchLen - 1] != '*');

    if(doubleAsteriskError || noAsteriskError) return WILDCARD_INVALID;
    return 0;
}


/**
 * @brief 
 * Reads the inputFile line by line and performs the corresponding find/replace function
 * based on the input mode.
 * 
 * @param mode 
 *      Integer indicating which search/replace function should be performed.
 * 
 * @param start_end_lines 
 *      Array containing the starting/ending line numbers which indicate the range of the
 *      search/replace function.
 */
void findReplace(int mode, int *start_end_lines){
    char curr_line[MAX_LINE + 1];
    int lineNum = 1;
    while(fgets(curr_line, MAX_LINE, inputFile)){
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


/**
 * @brief 
 * Performs the normal find/replace function, where all existence of search_text would be replace 
 * by replace_text.
 * 
 * @param curr_line
 *      Line being read by the program.
 */
void replace(char *curr_line){
    //If search_text == replace_text, print the original line in the outputFile.
    if(!strcmp(search_text, replace_text)){
        fputs(curr_line, outputFile);
        return;
    }

    char *word_found = strstr(curr_line, search_text); //Find the first occurrence of search_text.
    //Loop until all search_line are replaced.
    while(word_found){
        int indexOfWord = (word_found - curr_line), lineLen = strlen(curr_line);
        
        /*
        Print all texts before search_text + replace_text.
        The index of search_text represents the number of characters that are before search_text.
        */
        fprintf(outputFile, "%.*s%s", indexOfWord, curr_line, replace_text);

        //when search_text is the last word in curr_line, stop the function.
        if(*(word_found + searchLen) == '\0') return;
        
        getRemainingTexts(curr_line, word_found + searchLen, lineLen + 1);
        word_found = strstr(curr_line, search_text);
    }
    fputs(curr_line, outputFile); //Prints all the leftover texts.
}


/**
 * @brief
 * Performs the prefix find/replace function, where all words with the input prefix would be
 * replace by replace_text.
 * 
 * @param curr_line
 *      Line being read by the program.
 * 
 * @param prefix
 *      Prefix of a word.
 */
void prefixReplace(char *curr_line, char *prefix){
    char *prefix_found = strstr(curr_line, prefix);
    while(prefix_found){
        int indexOfWord = prefix_found - curr_line, lineLen = strlen(curr_line), 
            endOfWordIndex = updateEndIndex(curr_line, lineLen, indexOfWord + searchLen - 1);


        //Check if the prefix found is a true prefix, meaning it must be at the beginning of a word.
        if(indexOfWord == 0 || !isalnum(curr_line[indexOfWord - 1]))
            fprintf(outputFile, "%.*s%s", indexOfWord, curr_line, replace_text);
        else
            fprintf(outputFile, "%.*s", endOfWordIndex, curr_line);
        
        if(*(curr_line + endOfWordIndex) == '\0') return;
        
        getRemainingTexts(curr_line, curr_line + endOfWordIndex, lineLen + 1);
        prefix_found = strstr(curr_line, prefix);
    }
    fputs(curr_line, outputFile);
}


/**
 * @brief
 * Performs the suffix find/replace function, where all words with the input suffix would be replace
 * by replace_text.
 * 
 * @param curr_line
 *      Line being read by the program.
 * 
 * @param suffix 
 *      Suffix of a word.
 */
void suffixReplace(char *curr_line, char *suffix){
    char *suffix_found = strstr(curr_line, suffix);
    while(suffix_found){
        int indexOfWord = suffix_found - curr_line, lineLen = strlen(curr_line),
            endOfWordIndex = updateEndIndex(curr_line, lineLen, indexOfWord + searchLen - 1);
        
        
        //Check if the suffix found is a true suffix, meaning it must be at the end of the word.
        if(!strncmp(suffix, curr_line + (endOfWordIndex - (searchLen - 1)), searchLen - 1)){
            int startOfWordIndex = updateStartIndex(curr_line, indexOfWord - 1);
            fprintf(outputFile, "%.*s%s", startOfWordIndex + 1, curr_line, replace_text);
        }
        else fprintf(outputFile, "%.*s", endOfWordIndex, curr_line);

        if(*(curr_line + endOfWordIndex) == '\0') return;

        getRemainingTexts(curr_line, curr_line + endOfWordIndex, lineLen + 1);
        suffix_found = strstr(curr_line, suffix);
    }
    fputs(curr_line, outputFile);
}


/**
 * @brief 
 *      Obtains the index that indicates the ending of a word.
 *      For example, the ending index for "apple." is 5.
 * 
 * @param curr_line
 *      Line being read by the program.
 * 
 * @param lineLen
 *      Length or number of characters in the current line.
 * 
 * @param endIndex
 *      Initial ending index.
 * 
 * @return
 *      Updated ending index.
 */
int updateEndIndex(char *curr_line, int lineLen, int endIndex){
    while(endIndex < lineLen && isalnum(curr_line[endIndex])) endIndex++;
    return endIndex;
}


/**
 * @brief 
 *      Obtains the index that indicates the beginning of a word.
 *      For example, starting index for ". Apple" is 1.
 * 
 * @param curr_line 
 *      Line being read by the program.
 * 
 * @param startIndex
 *      Initial staring index.
 * 
 * @return
 *      Updated starting index.
 */
int updateStartIndex(char *curr_line, int startIndex){
    while(startIndex >= 0 && isalnum(curr_line[startIndex])) startIndex--;
    return startIndex;
}


/**
 * @brief
 * Updates curr_line with all the characters after search_text.
 * 
 * @param curr_line 
 *      Line being read by the program.
 * 
 * @param startOfRemain
 *      Starting index of the remaining texts.
 * 
 * @param lineLen
 *      Length or number of characters in the current line.
 *      
 */
void getRemainingTexts(char *curr_line, char *startOfRemain, int lineLen){
    char remain[lineLen];
    strcpy(remain, startOfRemain);
    strcpy(curr_line, remain);
}
