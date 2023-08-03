#include "FindReplace.h"
extern char *optarg;

int srFlag(bool *checkFlag, bool *checkArg, char **sr_texts){
    if(*checkFlag)
        return DUPLICATE_ARGUMENT;
    if(optarg == NULL || optarg[0] == '-')
        *checkArg = true;
    else{
        *sr_texts = optarg;
        *checkFlag = true;
    }
    return 0;
}

int scanArgs(int argc, char *argv, bool *checkFlags, bool *checkArgs, char **sr_texts){
    extern int opterr, optopt;
    int option, error = 0;
    opterr = 0;

    while((option = getopt(argc, argv, "s:r:l:w")) != -1){
        switch(option){

            case 's': {
                error = srFlag(checkFlags, checkArgs, sr_texts);
                break;
            }
            case 'r': {
                error = srFlag(checkFlags + 1, checkArgs + 1, sr_texts + 1);
                break;
            }
            case 'l': {
                break;
            }
            case 'w': {
                if(checkFlags[3])
                    error = DUPLICATE_ARGUMENT;
                else
                    checkFlags[3] = true;
                break;
            }
            if(error)
                return error;
        }
    }
}

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
    checkFlags checks if a flag is entered by the user, where checkFlags = {s flag, r flag, l flag, w flag}

    checkArgs checks if an argument is missing/invalid for a specific flag, where checkArgs = {s flag arg, r flag arg, l flag arg}
    */
    bool checkFlags[] = {false, false, false, false}, checkArgs = {false, false, false};
    
    //sr_texts indicates the search and replace test, where sr_texts = {search text, repalcement text}
    char *sr_texts[2];

    int result = scanArgs(argc, argv, checkFlags, checkArgs, sr_texts);
    if(result)
        return result;

    return 0;
}
