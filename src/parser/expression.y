%{
#include <iostream>
#include <string>
#include <map>
#include "../expression.h"

int yylex(void);
void yyerror(const char *);

Tree* result = nullptr;
map <string, int> variables;
%}

%union {
    Tree* tree;
}

%token<tree> IMP OR AND NEG
%token<tree> LEFT RIGHT
%token<tree> NAME
%token END

%right IMP
%left OR
%left AND
%left NEG

%type<tree> Input Expression

%start Input

%%
Input: Expression { result = $1; }
;

Expression: NAME { $$=$1; variables[$1->val]=0; }
| Expression IMP Expression { $$=new Tree('>', $1, $3); }
| Expression OR Expression { $$=new Tree('|', $1, $3); }
| Expression AND Expression { $$=new Tree('&', $1, $3); }
| NEG Expression { $$=new Tree('!', $2, NULL); }
| LEFT Expression RIGHT { $$=$2; }
;

%%
