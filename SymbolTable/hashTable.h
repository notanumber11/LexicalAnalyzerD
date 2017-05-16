

#ifndef COMPILADORES_HASHTABLE_H_H
#define COMPILADORES_HASHTABLE_H_H


#endif //COMPILADORES_HASHTABLE_H_H


typedef struct  {
    int lexicalComponent;
    char *lexema;
}SymbolEntry;

// IN ORDER TO PRINT THE NUMBER OF KEYWORDS IN THE TABLE
void setNumberOfKeyWords(int number);

void initTable(int size);

void deleteTable();

void display();

void insertItem(SymbolEntry item);

SymbolEntry *search(char lexema[256]);

int deleteItem(SymbolEntry *item);

int sizeLexema(char* lexema);