

#include "lexicalAnalyzer.h"
#include "../Definitions/definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include "../SymbolTable/hashTable.h"
#include "../ErrorManager/errorManager.h"


typedef  enum mainAutomaton{ BEGIN, LETTER};
typedef enum numberAutomaton {NUMBER,DOUBLE_NUMBER,BINARY_NUMBER,EXPONENCIAL1,EXPONENCIAL2};
typedef  enum commentAutomaton{ BEGIN_COMMENT, CONFIRM_COMMENT,END_MULTIPLE_COMMENT,CONFIRM_END_MULTIPLE_COMMENT,END_HIERARCHICAL,CONFIRM_END_HIERARCHICAL,NEW_HIERARCHICAL};

int line = 0;

// V A R I A B  L E S
int state;
char c;
char *buffer;
int pos;


void endLexicalAnalyzer(){
    free(buffer);
}

void startLexicalAnalyzer(){
    buffer = (char *) malloc(sizeof(char) * SIZE_BLOCK);
}



///This method include { (,),[,],{,} }
int automatonBrackets() {
    if (c == '(') {
        printf("LEX: [ %s ] LC [ %d ] \n", buffer, LEFT_BRACKET);
        return LEFT_BRACKET;
    }
    else if (c == ')') {
        printf("LEX: [ %s ] LC [ %d ] \n", buffer, RIGHT_BRACKET);
        return RIGHT_BRACKET;
    }
    else if (c == '[') {
        printf("LEX: [ %s ] LC [ %d ] \n", buffer, SQUARE_LEFT_BRACKET);
        return SQUARE_LEFT_BRACKET;
    }
    else if (c == ']') {
        printf("LEX: [ %s ] LC [ %d ] \n", buffer, SQUARE_RIGHT_BRACKET);
        return SQUARE_RIGHT_BRACKET;
    }
    else if (c == '{') {
        printf("LEX: [ %s ] LC [ %d ] \n", buffer, CURLY_LEFT_BRACKET);
        return CURLY_LEFT_BRACKET;
    }
    else if (c == '}') {
        printf("LEX: [ %s ] LC [ %d ] \n", buffer, CURLY_RIGHT_BRACKET);
        return CURLY_RIGHT_BRACKET;
    }

    return -1;
}

char operatorsArray[7] = {'+','-','*','/','<','>','\0'};
int automatonOperators() {
    int i = 0;
    while(operatorsArray[i]!='\0'){
        if(c==operatorsArray[i]){
            printf("LEX: [ %s ] LC [ %d ] \n", buffer, operatorsArray[i]);
            return operatorsArray[i];
        }
        i++;
    }

    if (c == '=') {
        nextCharacter();
        if(c!='='){
            previousCharacter();
            printf("LEX: [ %s ] LC [ %d ] \n", buffer, OPERATION_ASSIGN);
            return OPERATION_ASSIGN;
        }
        if(c == '='){
            printf("LEX: [ %s ] LC [ %d ] \n", buffer, OPERATION_EQUALS);
            return OPERATION_EQUALS;
        }
    }
    return -1;
}


char separatorsArray[7] = {',',';',' ','.','\n','\0'};
int automatonSeparators(){
    int i = 0;
    while(separatorsArray[i]!='\0'){
        if(c==separatorsArray[i]){
            if(c!='\n' && c!=' '){
                printf("LEX: [ %s ] LC [ %d ] \n", buffer, separatorsArray[i]);
                return separatorsArray[i];

            }
            if(c=='\n'){
                line++;
            }
            if(c=='\n' | c==' ') {
                return nextLexicalComponent();
            }
        }
        i++;
    }
    return -1;
}

int automatonStrings(){
    int stateComment = 0;
    while(1){
        switch(stateComment){
            case 0:
                if(c=='"'){
                    stateComment = 1;
                }
                else{
                    return -1;
                }
                break;
            case 1:
                if(c=='"'){
                    printf("LEX: [ %s ] LC [ %d ] \n", buffer, STRING);
                    return STRING;
                }
                else if(c==92){
                    nextCharacter();
                }
                else if(c=='\n'){
                    line++;
                    printError(STRING_DOES_NOT_END,line,buffer);
                    return  -1;
                }
                else if(c==END_OF_FILE){
                    //printf("Error: [ %s ] LC [ %d ] \n", "String no cerrada \n", STRING);
                    printError(STRING_JUMP_LINE,line,buffer);
                    return END_OF_FILE;
                }
                break;
        }
        nextCharacter();
    }
}


int automatonNumbers(){
    int stateNumber = 0;
    while(1){
        nextCharacter();
        //If we arrive to a delimiter
        if(!isalpha(c) && !isdigit(c) && c!='_' && c!='.'){
            buffer[pos-1] = '\0';
            previousCharacter();
            switch (stateNumber){
                case 0:
                    printf("LEX: [ %s ] LC [ %d ] \n", buffer, NUMBER_INT);
                    return NUMBER_INT;
                case 1:
                    printf("LEX: [ %s ] LC [ %d ] \n", buffer, NUMBER_DOUBLE);
                    return NUMBER_DOUBLE;
                case 2:
                    printf("LEX: [ %s ] LC [ %d ] \n", buffer, NUMBER_BINARY);
                    return NUMBER_BINARY;
                case 4:
                    printf("lEX: [ %s ] LC [ %d ] \n", buffer, NUMBER_EXPONENTIAL);
                    return NUMBER_EXPONENTIAL;
            }
        }
        switch (stateNumber){
            case 0: // INT
                if(isdigit(c) || c == '_'){
                    stateNumber = NUMBER;
                }
                else if(c == '.'){
                    stateNumber = DOUBLE_NUMBER;
                }
                else if(c == 'b'){
                    stateNumber = BINARY_NUMBER;
                }
                else if(c=='e'){
                    stateNumber =EXPONENCIAL1;
                }
                else{
                    printError(BAD_FORMATED_NUMBER,line,buffer);
                    return nextLexicalComponent();
                }
                break;
            case DOUBLE_NUMBER: //DOUBLE
                if(isdigit(c) || c== '_'){
                    stateNumber = DOUBLE_NUMBER;
                }
                else if(c=='e'){
                    stateNumber =EXPONENCIAL1;
                }
                else{
                    printError(BAD_FORMATED_DOUBLE,line,buffer);
                    return nextLexicalComponent();
                }
                break;
            case BINARY_NUMBER: //BINARY
                if(c=='1' || c=='0' || c=='_'){
                    stateNumber = BINARY_NUMBER;
                }
                else{
                    printError(BAD_FORMATED_BINARY,line,buffer);
                    return nextLexicalComponent();
                }
                break;
            case EXPONENCIAL1: //EXPONENCIAL1  example: 1.0e
                if(c=='+' || c=='-' || c=='_'){
                    stateNumber = EXPONENCIAL2;
                }
                else if(isdigit(c)){
                    stateNumber = EXPONENCIAL2;
                }
                else{
                    printError(BAD_FORMATED_EXP,line,buffer);
                    return nextLexicalComponent();
                }
                break;
            case EXPONENCIAL2: //EXPONENCIAL example: 1.0e+
                if(isdigit(c) || c== '_'){
                    stateNumber = EXPONENCIAL2;
                }
                else{
                    printError(BAD_FORMATED_EXP,line,buffer);
                    return nextLexicalComponent();
                }
                break;
        }
    }
}

int automatonComments(){
    int stateComment = 0;
    int pila = 0;
    while(1){
        if(c==END_OF_FILE){
            previousCharacter();
            printError(COMMENT_DOES_NOT_END,line,buffer);
            return END_OF_FILE;
        }
        if(c=='\n'){
            line ++;
        }
        switch (stateComment){
            case BEGIN_COMMENT:
                if(c=='/'){ //First char of a comment
                    stateComment = CONFIRM_COMMENT;
                }
                else{
                    return -1;
                }
                break;
            case CONFIRM_COMMENT:
                if(c=='*'){ //Multiple comment /*
                    stateComment = END_MULTIPLE_COMMENT;
                }
                else if(c=='+'){ //Multiple comment /+
                    stateComment = END_HIERARCHICAL;
                    pila++;
                }
                else if (c=='/'){
                    nextCharacter();
                    if(c=='/'){
                        while(c!='\n'){
                            nextCharacter();
                        }
                        previousCharacter();
                        printf("LEX: [ %s ] LC [ %d ] \n", buffer, DOCUMENT_LINE_COMMENT);
                        return  DOCUMENT_LINE_COMMENT;
                    }
                    else{
                        while(c!='\n'){
                            nextCharacter();
                        }
                        previousCharacter();
                        printf("LEX: [ %s ] LC [ %d ] \n", buffer, LINE_COMMENT);
                        return  LINE_COMMENT;
                    }

                }
                else{
                    previousCharacter();
                    previousCharacter();
                    nextCharacter();
                    return -1;
                }

                break;
            case END_MULTIPLE_COMMENT:
                if(c=='*'){ //Multiple comment /*
                    stateComment = CONFIRM_END_MULTIPLE_COMMENT;
                }
                break;
            case CONFIRM_END_MULTIPLE_COMMENT:
                if(c=='/'){  // Multiple comment */
                    printf("LEX: [ %s ] LC [ %d ] \n", buffer, COMMENT_MULTIPLE);
                    return  COMMENT_MULTIPLE;
                }
                else{
                    stateComment = END_MULTIPLE_COMMENT;
                }
                break;

            case END_HIERARCHICAL:
                if(c=='+'){ //Multiple comments /+ +
                    stateComment = CONFIRM_END_HIERARCHICAL;
                }
                else if (c=='/'){
                    stateComment = NEW_HIERARCHICAL;
                }
                break;
            case CONFIRM_END_HIERARCHICAL:
                if(c=='/'){//Multiple comments /+ +/
                    pila--;
                    if(pila==0){
                        printf("LEX: [ %s ] LC [ %d ] \n", buffer, COMMENT_HIERARCHIC);
                        return  COMMENT_HIERARCHIC;
                    }
                    else{
                        stateComment = END_HIERARCHICAL;
                    }
                }
                break;
            case NEW_HIERARCHICAL:
                if(c=='+'){
                    stateComment = END_HIERARCHICAL;
                    pila++;
                }
                else{
                    stateComment = END_HIERARCHICAL;
                }
                break;
            default:
                printf("En el caso default de comentarios\n");
        }
        nextCharacter();
    }
    return -1;
}



int nextLexicalComponent() {

    state = 0;
    pos = 0;

    buffer[0] = '\0';

    while (1) {
        nextCharacter();
        switch (state) {
            case BEGIN:
                //END OF FILE
                if (c == END_OF_FILE) {
                    printf("\n END OF FILE -- [ %s ] LC [ %d ] \n", "END", END_OF_FILE);
                    return END_OF_FILE;
                }
                //This else analyze separators, comments, operators, brackets and strings
                else if (!isalpha(c) && !isdigit(c) && c!='_') {
                    //printf("\n Delimiter |%s| \n", buffer);
                    int flag=0;
                    flag = automatonSeparators();
                    if(flag == -1){
                        flag = automatonComments();
                    }
                    if(flag==-1){
                        flag = automatonOperators();
                    }
                    if(flag==-1){
                        flag = automatonBrackets();
                    }
                    if(flag==-1){
                        flag = automatonStrings();
                    }
                    if(flag ==-1){
                        printError(UNEXPECTED_CHAR,line,buffer);
                        return nextLexicalComponent();
                    }
                    return flag;
                }
                    //START BY DIGIT
                else if (isdigit(c)) {
                    return automatonNumbers();
                }
                    //START BY LETTER
                else if (isalpha(c) || c == '_') {
                    state = LETTER;
                }
                //printf("Nos encontramos en el estado [ 0 ] con buffer [ %s ]\n", buffer);
                break;
            case LETTER: //LETTER (we arrive here only with a previous LETTER read)
                if (!isalpha(c) && !isdigit(c) && c!='_') {
                    buffer[pos - 1] = '\0';
                    previousChar();
                    SymbolEntry *s;
                    s = search(buffer);
                    if(s!=NULL){
                        printf("LEX: [ %s ] LC [ %d ] \n", buffer, s->lexicalComponent);
                        return s->lexicalComponent;
                    }
                    else{
                        s = (SymbolEntry*)malloc(sizeof(SymbolEntry));

                        s->lexema = (char*)malloc(sizeof(char)*sizeLexema(buffer));
                        strcpy(s->lexema,buffer);
                        s->lexicalComponent = IDENTIFIER;
                        insertItem(*s);
                        printf("LEX: [ %s ] LC [ %d ] \n", buffer, IDENTIFIER);
                        free(s->lexema);
                        free(s);
                        return  IDENTIFIER;
                    }
                }
                if (isalpha(c) | c == '_') {
                    state = LETTER;
                }
                break;
            default:
                printError(UNEXPECTED_CHAR,line,buffer);
                return nextLexicalComponent();
        }
    }
}



void nextCharacter(){
    c= nextChar();
    buffer[pos] = c;
    buffer[pos+1] = '\0';
    pos++;
}



void previousCharacter(){
    previousChar();
    pos--;
    buffer[pos]='\0';
}

