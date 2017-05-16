#include "SymbolTable/hashTable.h"
#include "SymbolTable/keywordsLoader.h"
#include "InputSystem/InputSystem.h"
#include "SintaticAnalyzer/SintactitAnalyzer.h"
#include "Definitions/definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include "Flex/lex.yy.c"

char pathFileToAnalyze[255] = "../Input/regression.d";
char pathFileDefinitions[255] = "../Definitions/definitions.h";


int main(int argc, char *argv[] ){
    int option = 0;
    if( argc == 2 ) {
        option = atoi(argv[1]);
    }
    else if( argc > 2 ) {
        printf("Only 1 argument, 0 or 1 is expected.\n");
       return -1;
    }
    else {

    }
    if(option == 0){
        myLexicalAnalyzer();
    }
    else if (option == 1){
        flexAnalyzer();
    }
}


int flexAnalyzer(){
    // INIT THE HASH TABLE
    initTable(INIT_SIZE_HASH_TABLE);

    //LOAD THE KEYOWORDS FROM DEFINITIONS.H
    if(loadKeyWordsFromFile(pathFileDefinitions)==-1){
        printf("-- ERROR -- File not found [ %s ]\n",pathFileDefinitions);
        return EXIT_FAILURE;
    }



    yyin = fopen(pathFileToAnalyze, "r");


    while(yylex()){
    }

    fclose(yyin);

    printf( "Numero de lıneas = %d\n", num_lineas);


    //DISPLAY DE HASH TABLE
    display();

    // DELETE THE HASH TABLE
    deleteTable();
    return 0;
}

int myLexicalAnalyzer(){
    // INIT THE FILE SYSTEM
    if(initInputSystem(pathFileToAnalyze)==-1){
        printf("-- ERROR -- File not found [ %s ]\n",pathFileToAnalyze);
        return EXIT_FAILURE;
    }

    // INIT THE HASH TABLE
    initTable(INIT_SIZE_HASH_TABLE);

    //LOAD THE KEYOWORDS FROM DEFINITIONS.H
    if(loadKeyWordsFromFile(pathFileDefinitions)==-1){
        printf("-- ERROR -- File not found [ %s ]\n",pathFileDefinitions);
        return EXIT_FAILURE;
    }

    //START THE ANALYZER
    startSintatic();

    int analizyng = -1;
    while(analizyng != END_OF_FILE){
        analizyng = nextSintaticComponent();
    }

    // END THE ANALYZER
    endSintatic();

    //DISPLAY DE HASH TABLE
    display();

    // DELETE THE HASH TABLE
    deleteTable();

    // FREE MEMORY OF INPUT SYSTEM
    endInputSystem();
}