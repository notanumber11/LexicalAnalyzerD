cmake_minimum_required(VERSION 3.6)
project(Compiladores)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(SOURCE_FILES main.c Definitions/definitions.h SymbolTable/keywordsLoader.c SymbolTable/keywordsLoader.h
        InputSystem/InputSystem.c InputSystem/InputSystem.h SymbolTable/hashTable.c
        SymbolTable/hashTable.h LexicalAnalyzer/lexicalAnalyzer.c LexicalAnalyzer/lexicalAnalyzer.h
        ErrorManager/errorManager.c ErrorManager/errorManager.h SintaticAnalyzer/SintactitAnalyzer.c
        SintaticAnalyzer/SintactitAnalyzer.h )

add_executable(Compiladores ${SOURCE_FILES} )

