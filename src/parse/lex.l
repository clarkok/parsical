%{
#include "../parsical-parser.hpp"
#include "parse.tab.hpp"
%}
ID      ([a-zA-Z][_a-zA-Z0-9]*)|"_"
WHITE   [ \t\v\r\n]
ESCAPED \\([abfnrtv\\\'\"?]|[0-7][0-7][0-7]|x[0-9a-fA-F][0-9a-fA-F])
STRING  \"([^\\\"]|{ESCAPED})+\"
REGEX   "/"([^\\/]|("\\".))+"/"
%%
{ID}        { yylval.literal = yytext; return ID; }
{WHITE}     { if (*yytext == '\n') ++yylineno; }
{STRING}    { yylval.literal = yytext; return STRING; }
{REGEX}     { yylval.literal = yytext; return REGEX; }
.           { return *yytext; }
%%
int yywrap()
{ return 1; }
