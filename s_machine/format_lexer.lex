%{
#include <string.h>
#include "format_parser.h"
#include "s_machine.h"

Value lexval;
Str_c_node **  format_stringtable;
%}
%option noyywrap
spacing     ([ \t])+
digit       [0-9]
letter      [A-Za-z]
atomic      [cirsb]
id			{letter}({letter}|{digit})*
sugar       [\[\]\(\)\:\,]
%%
{spacing}   ;
{digit}+    {lexval.ival = atoi(yytext); return(FORMAT_LEX_INT);}
{sugar}     {return yytext[0];}
{atomic}    {lexval.cval = yytext[0]; return yytext[0];}
{id}        {lexval.sval = insert_str_c(yytext); return FORMAT_LEX_ID;}
.           {return(FORMAT_LEX_ERROR);}
%%