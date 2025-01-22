/******************************************************************* 
Name: Caleb Hickman-Thomas NetID: cjh767
Course: CSE 4714 Assignment: Part 1
Programming Environment: VSCODE 
Purpose of File: Contains lex rules for programming project part 1 which are all shown below and commented
to show what sections cover what rules based on the table given by the instructor.
*******************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

%}

%%


"BEGIN"  { return TOK_BEGIN; }
"BREAK"  { return TOK_BREAK; }
"CONTINUE" { return TOK_CONTINUE; }
"DOWNTO" { return TOK_DOWNTO; }
"ELSE" { return TOK_ELSE; }
"END" { return TOK_END; } 
"FOR" { return TOK_FOR; }
"IF" { return TOK_IF;}
"LET" { return TOK_LET; } 
"PROGRAM" { return TOK_PROGRAM; } 
"READ" { return TOK_READ; } 
"THEN" {return TOK_THEN; }
"TO" { return TOK_TO; }
"VAR" {return TOK_VAR; } 
"WHILE" { return TOK_WHILE; }
"WRITE" { return TOK_WRITE; }


"INTEGER" { return TOK_INTEGER; }
"REAL" { return TOK_REAL; }


";" { return TOK_SEMICOLON; }
":" { return TOK_COLON; } 
"(" { return TOK_OPENPAREN; }
")" { return TOK_CLOSEPAREN; }

\+ { return TOK_PLUS; }
\- { return TOK_MINUS; }
\* { return TOK_MULTIPLY; }
\/ { return TOK_DIVIDE; }
\:= { return TOK_ASSIGN; }
\= { return TOK_EQUALTO; }
\< { return TOK_LESSTHAN; }
\> { return TOK_GREATERTHAN; }
\<> { return TOK_NOTEQUALTO; }
"MOD" { return TOK_MOD; }
"NOT" { return TOK_NOT; }
"OR" { return TOK_OR; }
"AND" { return TOK_AND; }

[A-Z][A-Z0-9]* {
    if (yyleng > 8) {
        return TOK_UNKNOWN;
    } else {
        return TOK_IDENT;
    }
}

[0-9]+ { return TOK_INTLIT; }
'[^'\n]{0,80}'   { return TOK_STRINGLIT; }
[0-9]+\.[0-9]+    { return TOK_FLOATLIT; }

"{"[^}]*"}" 
[ \t\r\n]+ 




 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }
