%option noyywrap
%{
#include <stdio.h>
#include "../Definitions/definitions.h"
#include "../ErrorManager/errorManager.h"
int num_lineas = 1 ;
int pilaComments = 0;
%}

DIGITO [0-9]_*
%x BlockComment
%x BlockDocument
%x HierarchicComment
%x stringState
%%

"/+"                                                        BEGIN(HierarchicComment);pilaComments++; yymore();
<HierarchicComment>"/+"                                     pilaComments++; yymore();
<HierarchicComment>"+/"                                     {pilaComments--;
                                                                 if(pilaComments==0){printf(" Linea %d -->COMMENT_HIERARCHIC [ %s ]\n", num_lineas, "[comment here]"); BEGIN(INITIAL); return COMMENT_HIERARCHIC;}else{yymore();}}

<HierarchicComment>[^+/\n]*                                  /* Detecta Lo que no es + :Accede comentario jerarquico */ yymore();
<HierarchicComment>"+"+[^*/\n]*                             /* Detecta + que no continuan por / */ yymore();

<HierarchicComment>"/"+[^*+\n]*                             /* Detecta + que no continuan por / */ yymore();




"/**"+[^"/"\n]                                              BEGIN(BlockDocument); yyless(1); yymore();
"/*"                                                        BEGIN(BlockComment); yymore();

<BlockComment>"*/"                                          printf(" Linea %d -->COMMENT_MULTIPLE [ %s ]\n", num_lineas,"[comment here]"); BEGIN(INITIAL);
<BlockDocument>"*"+"/"                                      printf(" Linea %d -->DOCUMENT_MULTIPLE_COMMENT [ %s ]\n", num_lineas,"[comment here]"); BEGIN(INITIAL); return DOCUMENT_MULTIPLE_COMMENT;

<BlockComment,BlockDocument>[^*\n]*                         /* Detecta Lo que no es * :Accede comentario de bloque y de documentacion de bloque */ yymore();
<BlockComment,BlockDocument>"*"+[^*/\n]*                    /* Detecta * que no continuan por / */ yymore();


<BlockComment,BlockDocument,HierarchicComment>\n            /* Gestion de numero de lineas en comentarios / */     ++num_lineas; yymore();


"///"+[^\n]*                                                printf(" Linea %d -->DOCUMENT_LINE_COMMENT [ %s ]\n", num_lineas,"[comment here]");  return DOCUMENT_LINE_COMMENT;
"//"+[^\n]*                                                 printf(" Linea %d -->LINE_COMMENT [ %s ]\n", num_lineas,"[comment here]");


\"(\\.|[^\"])*\"                                printf(" Linea %d -->STRING [ %s ]\n", num_lineas,yytext); return STRING;
{DIGITO}*                                       printf(" Linea %d -->NUMBER_INT [ %s ]\n", num_lineas,yytext); return NUMBER_INT;
{DIGITO}++"."{DIGITO}*                          printf(" Linea %d -->NUMBER_DOUBLE [ %s ]\n",num_lineas,yytext); return NUMBER_DOUBLE;
"."{DIGITO}+                                    printf(" Linea %d -->NUMBER_DOUBLE [ %s ]\n",num_lineas,yytext); return NUMBER_DOUBLE;
0b[0-1]*                                        printf(" Linea %d -->NUMBER_BINARY [ %s ]\n",num_lineas,yytext); return NUMBER_BINARY;
{DIGITO}++"."*{DIGITO}*+"e"+[+-]*+{DIGITO}+     printf(" Linea %d -->NUMBER_EXPONENTIAL [ %s ]\n",num_lineas,yytext); return NUMBER_EXPONENTIAL;
[_a-zA-Z][_a-zA-Z0-9]*                          return isIdentifier();


"("                                             printf(" Linea %d -->LEFT_BRACKET [ %s ]\n",num_lineas,yytext); return LEFT_BRACKET;
")"                                             printf(" Linea %d -->RIGHT_BRACKET [ %s ]\n",num_lineas,yytext); return RIGHT_BRACKET;
"["                                             printf(" Linea %d -->SQUARE_LEFT_BRACKET [ %s ]\n",num_lineas,yytext); return SQUARE_LEFT_BRACKET;
"]"                                             printf(" Linea %d -->SQUARE_RIGHT_BRACKET [ %s ]\n",num_lineas,yytext); return SQUARE_RIGHT_BRACKET;
"{"                                             printf(" Linea %d -->CURLY_LEFT_BRACKET [ %s ]\n",num_lineas,yytext); return CURLY_LEFT_BRACKET;
"}"                                             printf(" Linea %d -->CURLY_RIGHT_BRACKET [ %s ]\n",num_lineas,yytext); return CURLY_RIGHT_BRACKET;
"="                                             printf(" Linea %d -->OPERATION_EQUALS [ %s ]\n",num_lineas,yytext); return OPERATION_EQUALS;
"=="                                            printf(" Linea %d -->OPERATION_ASSIGN [ %s ]\n",num_lineas,yytext); return OPERATION_ASSIGN;


"+"                                             printf(" Linea %d -->Operator  [ %s ]\n",num_lineas,yytext); return  '+';
"-"                                             printf(" Linea %d -->Operator  [ %s ]\n",num_lineas,yytext); return  '-';
"*"                                             printf(" Linea %d -->Operator  [ %s ]\n",num_lineas,yytext); return  '*';
"/"                                             printf(" Linea %d -->Operator  [ %s ]\n",num_lineas,yytext); return '/';
"<"                                             printf(" Linea %d -->Operator  [ %s ]\n",num_lineas,yytext); return  '<';
">"                                             printf(" Linea %d -->Operator  [ %s ]\n",num_lineas,yytext); return  '>';


","                                             printf(" Linea %d -->Separator  [ %s ]\n",num_lineas,yytext); return  ',';
";"                                             printf(" Linea %d -->Separator  [ %s ]\n",num_lineas,yytext); return  ';';
"."                                             printf(" Linea %d -->Separator  [ %s ]\n",num_lineas,yytext); return  '.';
"\\n"                                           printf(" Linea %d -->Separator  [ %s ]\n",num_lineas,yytext); return  '\n';
"\\0"                                           printf(" Linea %d -->Separator  [ %s ]\n",num_lineas,yytext); return  '\0';


\n                                                ++num_lineas;
" "
\"                                              printError(STRING_DOES_NOT_END,num_lineas-1,yytext);
.                                               printError(UNEXPECTED_CHAR,num_lineas-1,yytext);
<BlockComment,BlockDocument,HierarchicComment><<EOF>>       printError(COMMENT_DOES_NOT_END,num_lineas-1,yytext);
%%

int yyerror(const char *msg) {
    printf("Pringaos");
    return -1;
}
int isIdentifier();
int isIdentifier(){
            SymbolEntry *s;
            s = search(yytext);
            if(s!=NULL){
                    if(s->lexicalComponent==505){
                        printf(" Linea %d -->IDENTIFIER  [ %s ]\n",num_lineas,yytext);
                    }
                    else{
                        printf(" Linea %d -->KEYWORD  [ %s ]\n",num_lineas,yytext);
                    }
                 //printf("LEX: [ %s ] LC [ %d ] \n", yytext, s->lexicalComponent);
                 return s->lexicalComponent;
            }
            else{
                s = (SymbolEntry*)malloc(sizeof(SymbolEntry));
                s->lexema = (char*)malloc(sizeof(char)*sizeLexema(yytext));
                strcpy(s->lexema,yytext);
                s->lexicalComponent = IDENTIFIER;
                insertItem(*s);
                free(s->lexema);
                free(s);
                  printf(" Linea %d -->IDENTIFIER  [ %s ]\n",num_lineas,yytext);
                 //printf("LEX: [ %s ] LC [ %d ] \n", yytext, IDENTIFIER);
                 return  IDENTIFIER;
            }
}
