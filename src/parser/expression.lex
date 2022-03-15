%{
#include <string>
#include "../expression.h"
#include "expression.tab.h"

%}

%option outfile="lex.c" header-file="lex.h"


quote "'"
white [ \t]*
bool [A-Z]+([A-Z]|[0-9]|{quote})*


%%
{bool} {
    yylval.tree=new Tree(yytext);
    return NAME;
}
{white} ;
"&" return AND;
"|" return OR;
"->" return IMP;
"!" return NEG;
"(" return LEFT;
")" return RIGHT;
"\n" return END;
%%
