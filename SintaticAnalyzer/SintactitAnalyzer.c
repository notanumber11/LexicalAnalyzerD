
#include "SintactitAnalyzer.h"
#include "../LexicalAnalyzer/lexicalAnalyzer.h"
#include "../Definitions/definitions.h"


void startSintatic(){
    startLexicalAnalyzer();

}

int nextSintaticComponent(){
   return nextLexicalComponent();
}

void endSintatic(){
    endLexicalAnalyzer();
}