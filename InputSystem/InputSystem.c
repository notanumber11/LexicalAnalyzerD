#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "InputSystem.h"
#include "../Definitions/definitions.h"

// BLOCK SYSTEM
FILE *rm;
char *actualBlock;
char *lastBlock;
int positionBlock = -1;
int flagLastBlock = 0;
int flagFirstBlock = -1;
int numberOfBytesRead;

// INIT BLOCK SYSTEM
int initInputSystem(char* fileName){
    rm = fopen(fileName, "r");
    if(rm == NULL){
        return -1;
    }
    actualBlock = (char*)malloc(sizeof(char)*SIZE_BLOCK);
    lastBlock = (char*)malloc(sizeof(char)*SIZE_BLOCK);
    readBlock();
    return 0;
}


// END BLOCK SYSTEM
int endInputSystem(){
    free(actualBlock);
    free(lastBlock);
    fclose(rm);
}


//  READING A BLOCK
int readBlock(){
    positionBlock = -1;
    // CHECK IF WE ARE IN THE END OF THE FILE
    if(flagLastBlock){
        return -1;
    }

    flagFirstBlock++;
    if(flagFirstBlock>0){
        strcpy(lastBlock,actualBlock);
    }

    numberOfBytesRead = fread(actualBlock, 1, (SIZE_BLOCK)-1, rm);
    actualBlock[numberOfBytesRead] = '\0';

    //CHECK IF WE ARE IN THE SECOND BLOCK


    // CHECK IF WE ARE IN THE LAST BLOCK
    if(numberOfBytesRead<(SIZE_BLOCK-1)){
        flagLastBlock = 1;
        actualBlock[numberOfBytesRead] = END_OF_FILE;
    }
    return 1;
}



char nextChar(){
    positionBlock++;
    if(positionBlock<0){
        if(flagFirstBlock<1){
            return -1;
        }
        return lastBlock[numberOfBytesRead+positionBlock];

    }

    if(positionBlock>=(SIZE_BLOCK-1)){
        readBlock();
        nextChar();
    }
    return actualBlock[positionBlock];}

void previousChar(){
positionBlock--;
}

