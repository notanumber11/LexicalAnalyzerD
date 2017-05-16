
#include <stdio.h>
#include <stdlib.h>
#include "errorManager.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"


void printError(int errorCode,int line, char *lexema){

    for(int i= 0;i<SIZE_BLOCK;i++){
        if(lexema[i] == '\n'){
            lexema[i] = ' ';
        }
    }
    if(SIZE_BLOCK>30){
        lexema[30] = '\0';
    }
    printf("%sError:  --  Linea [ %d ]  -- Code [ %d ] --  Flex [ %s ] ... \n", KRED,line+1,errorCode,lexema);
    printf("%s",KNRM);
}