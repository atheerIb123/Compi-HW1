%{
#include "tokens.hpp"
#include <stdlib.h>
%}

%option noyywrap
%option yylineno


binop 			[-+*/]
comment						(\/\/[^\n\r]*)
identifier					([a-zA-Z][a-zA-Z0-9_]*)
number				([1-9]+[0-9]*|0)
whitespace  				([\t\n\r ])

string					(\"(\\\")?(([^\\\"\n](\\\\)*\\\")|[^\"\n])*\")
unclosed_str			(\"[^\"]*\n)

hex_ascii		(\\x([2-6][0-9a-fA-f]|7[0-9a-eA-E]))
escape_squance				((\\\")|(\\\\)|(\\r)|(\\t)|(\\n)|(\\0)|{hex_ascii})
relop				((!=)|(==)|(>=)|(<=)|>|<)

%%
void				return VOID;
int					return INT;
byte				return BYTE;
b					return B;
bool				return BOOL;
and					return AND;
or					return OR;
not					return NOT;
true				return TRUE;
false				return FALSE;
return				return RETURN;
if					return IF;
else				return ELSE;
while				return WHILE;
break				return BREAK;
continue			return CONTINUE;
;					return SC;
\(					return LPAREN;
\)					return RPAREN;
\{					return LBRACE;
\}					return RBRACE;
=					return ASSIGN;
{relop}	return RELOP;
{binop}	return BINOP;
{comment}			return COMMENT;
{identifier}		return ID;
{number}	return NUM;
{string}		return STRING;
{unclosed_str}	return UNCLOSED;
{whitespace}                     ;       
.       			return ERROR;
%%
