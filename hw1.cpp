#include "tokens.hpp"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#define ILLEGAL_HEX_DIGIT -1

extern int yylex();
void printToLength(const char* buff, int length);
void printOutput(int line_num, const char* token_name , const char* token_value, int len_of_token_value);
void printToken(const char* token_name);
void illegalChar();
int hexToInt(char hex_digit);
char asciiToChar(char high_digit, char low_digit);
int isPrintableAsciiNum(int num);
int isInAsciiLegalRange(char c);
int legalHexSequence(const char* seq);
int formatString(const char* source, char* destination, int length);
void printStringToken();

int main(){
	int token;

	while(token = yylex()) 
	{
		switch(token){
			case VOID:
				printToken("VOID");
				break;
    		case INT:
				printToken("INT");
				break;
    		case BYTE:
				printToken("BYTE");
				break;
    		case B:
				printToken("B");
				break;
    		case BOOL:
				printToken("BOOL");
				break;
    		case AND:
				printToken("AND");
				break;
    		case OR:
				printToken("OR");
				break;
    		case NOT:
				printToken("NOT");
				break;
    		case TRUE:
				printToken("TRUE");
				break;
    		case FALSE:
				printToken("FALSE");
				break; 
    		case RETURN:
				printToken("RETURN");
				break; 
    		case IF:
				printToken("IF");
				break; 
    		case ELSE:
				printToken("ELSE");
				break; 
    		case WHILE:
				printToken("WHILE");
				break; 
    		case BREAK:
				printToken("BREAK");
				break; 
    		case CONTINUE:
				printToken("CONTINUE");
				break;
    		case SC:
				printToken("SC");
				break; 
    		case LPAREN:
				printToken("LPAREN");
				break; 
    		case RPAREN:
				printToken("RPAREN");
				break; 
    		case LBRACE:
				printToken("LBRACE");
				break; 
    		case RBRACE:
				printToken("RBRACE");
				break; 
    		case ASSIGN:
				printToken("ASSIGN");
				break; 
    		case RELOP:
				printToken("RELOP");
				break; 
    		case BINOP:
				printToken("BINOP");
				break; 
    		case COMMENT:
				printOutput(yylineno, "COMMENT", "//", -1);
				break; 
    		case ID:
				printToken("ID");
				break; 
    		case NUM:
				printToken("NUM");
				break; 
    		case STRING:
				printStringToken();
				break;
			case ERROR:
				std::cout << "Error " << yytext << std::endl;
				exit(0);				
				break;
		}
	}

	return 0;
}

void printToLength(const char* buff, int length)
{
	for(int i = 0; i < length; ++i)
	{
		std::cout << buff[i];
	}

}

void printOutput(int line, const char* token, const char* value, int length)
{
	std::cout << line << " " << token << " ";

	if(length != -1)
	{
		for(int i = 0; i < length; ++i)
		{
			std::cout << value[i];
			if(i == length-1)
				std::cout << std::endl;
		}
	} 
	else
	{
		std::cout << value << std::endl;
	}
}

void printToken(const char* token_name)
{
    printOutput(yylineno, token_name, yytext, -1);
}

int hexToInt(char digit){
	if ('0' <= digit && digit <= '9')
		return digit - '0';
	else if ('A' <= digit && digit <= 'F')
		return digit - 'A' + 10;
  else if ('a' <= digit && digit <= 'f')
    return digit - 'a' + 10;
    
	return ILLEGAL_HEX_DIGIT;
}

char asciiToChar(char first, char second)
{
	int res = 16*hexToInt(first) + hexToInt(second);
	return (char)res;
}


int isPrintableAsciiNum(int num){
	return (0x20 <= num && num <= 0x7e) || num == 0x09 || num == 0x0a || num == 0x0d;
}

int isInAsciiLegalRange(char c){
	return 0x00 <= c && c <= 0x7f;
}

int legalHexSequence(char* seq)
{
	if (seq[0] != 'x' && seq[1])
		return 0;

	if (seq[1] == '\0' || seq[2] == '\0')
		return 0;

	if (hexToInt(seq[1]) == ILLEGAL_HEX_DIGIT || hexToInt(seq[2]) == ILLEGAL_HEX_DIGIT)
		return 0;

	return isInAsciiLegalRange(asciiToChar(seq[1], seq[2]));
}

int formatString(const char* source, char* destination, int length)
{
	const char* const start = destination;
	const char* end = source + length - 1;

	source++;

	while(source != end)
	{
		if(*source == '\n')
		{
			std::cout << "Error unclosed string" << std::endl;
			exit(0);
		}
		if(*source != '\\'){
			*destination = *source;
		} 
		else
		{
			source++;

			if(source == end)
			{
				std::cout << "Error unclosed string" << std::endl;
				exit(0);
			}

			char escape_sequence[4] = "\0\0\0";

			if(*source == 'x'){
				escape_sequence[0] = 'x';

				for(int i = 1; source+i <= end && source[i] != '\"' && i <= 2; i++)
					escape_sequence[i] = source[i];

				
				if(!legalHexSequence(escape_sequence))
				{

					std::cout << "Error undefined escape sequence" << " " << escape_sequence << std::endl;
					exit(0);
				}

				char first = *(++source), second = *(++source);
				*destination = asciiToChar(first, second);
			} 
			else 
			{
				switch(*source)
				{
				case '\\':
					*destination = '\\';
					break;
				case '0':
					*destination = '\0';
					break;
				case 'r':
					*destination = '\r';
					break;
				case '\"':
					*destination = '\"';
					break;
				case 'n':
					*destination = '\n';
					break;
				
				case 't':
					*destination = '\t';
					break;
				
				default:
					std::cout << "Error undefined escape sequence" << " " << *source << std::endl;
					exit(0);
				}
			}
		}
		source++;
		destination++;
	}

	if(*end == '\n')
	{
		std::cout << "Error unclosed string" << std::endl;
		exit(0);
	}

	*destination = '\0';

	unsigned long int result = (unsigned long int)(destination)-(unsigned long int)(start);

	return result;
}

void printStringToken()
{
	char buff[4096];
	int result_length = formatString(yytext, buff, yyleng);

	printOutput(yylineno, "STRING", buff, result_length);
}