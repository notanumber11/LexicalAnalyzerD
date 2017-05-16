
#include <stdio.h>
#include "keywordsLoader.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hashTable.h"

SymbolEntry *item;




int loadKeyWordsFromFile(char *path){
    item = (SymbolEntry*) malloc((sizeof(SymbolEntry)));
    item->lexema = (char*) malloc(sizeof(char)*255);
    //Open the file
    FILE* fp = fopen(path, "r");
    if(fp==NULL){
        return -1;
    }
    char line[255];
    int i = 0;
//Flag to startLexicalAnalyzer the keywords in the file
    int beginKeyWords = 0;
//Delimiter word to startLexicalAnalyzer to read the keywords
    char keyword[255] = "//DelimiterKeywords\n";

    int numberOfKeyWords= 0;
//Read the file
    while (1) {
        if (fgets(line,150, fp) == NULL) break;
        i++;
        //Check if we are in the part of the keywords
        if(strcmp(keyword,line)==0){
            if(beginKeyWords==1){
                break;
            }
            else{
                beginKeyWords = 1;
                continue;
            }
        }

        //Parsing the keywords

        if(beginKeyWords){
            char *lexema;
            char *numberChar;
            int lexicalComponent;
            strtok (line," ");
            lexema = strtok(NULL," ");
            numberChar = strtok(NULL," ");
            for(int i = 0; lexema[i]; i++)
                lexema[i] = tolower(lexema[i]);

            lexicalComponent = atoi(numberChar);
            //printf("El resultado lexema = %s, lexicalComponent = %d\n",lexema,lexicalComponent);
            strcpy(item->lexema,lexema);
            item->lexicalComponent = lexicalComponent;
           insertItem(*item);
            numberOfKeyWords++;
        }
    }
    setNumberOfKeyWords(numberOfKeyWords);
    fclose(fp);
    free(item->lexema);
    free(item);
    return 0;
}