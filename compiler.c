#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"

#define DEBUG 1     // 1 => Turn ON debugging, 0 => Turn OFF debugging


void print_debug(const char * format, ...)
{
	va_list args;
	if (DEBUG)
	{
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
	}
}

//---------------------------------------------------------
// Lexer

char token[MAX_TOKEN_LENGTH];      // token string
int  ttype;                        // token type
int  activeToken = FALSE;
int  tokenLength;
int  line_no = 1;

char *reserved[] =
{ "",
"VAR",
"IF",
"WHILE",
"SWITCH",
"CASE",
"DEFAULT",
"print",
"ARRAY",
"+",
"-",
"/",
"*",
"=",
":",
",",
";",
"[",
"]",
"(",
")",
"{",
"}",
"<>",
">",
"<",
"ID",
"NUM",
"ERROR"
};

void skipSpace()
{
	char c;

	c = getchar();
	line_no += (c == '\n');
	while (!feof(stdin) && isspace(c))
	{
		c = getchar();
		line_no += (c == '\n');
	}

	// return character to input buffer if eof is not reached
	if (!feof(stdin))
		ungetc(c, stdin);
}

int isKeyword(char *s)
{
	int i;

	for (i = 1; i <= KEYWORDS; i++)
		if (strcmp(reserved[i], s) == 0)
			return i;
	return FALSE;
}

int scan_number()
{
	char c;

	c = getchar();
	if (isdigit(c))
	{
		if (c == '0')
		{
			token[tokenLength] = c;
			tokenLength++;
		}
		else
		{
			while (isdigit(c))
			{
				token[tokenLength] = c;
				tokenLength++;;
				c = getchar();
			}
			ungetc(c, stdin);
		}
		token[tokenLength] = '\0';
		return NUM;
	}
	else
		return ERROR;
}

int scan_id_or_keyword()
{
	int ttype;
	char c;

	c = getchar();
	if (isalpha(c))
	{
		while (isalnum(c))
		{
			token[tokenLength] = c;
			tokenLength++;
			c = getchar();
		}
		if (!feof(stdin))
			ungetc(c, stdin);

		token[tokenLength] = '\0';
		ttype = isKeyword(token);
		if (ttype == 0)
			ttype = ID;
		return ttype;
	}
	else
		return ERROR;
}

void ungetToken()
{
	activeToken = TRUE;
}

int getToken()
{
	char c;

	if (activeToken)
	{
		activeToken = FALSE;
		return ttype;
	}

	skipSpace();
	tokenLength = 0;
	c = getchar();

	switch (c)
	{
	case '+': ttype = PLUS; break;
	case '-': ttype = MINUS; break;
	case '/': ttype = DIV; break;
	case '*': ttype = MULT; break;
	case '=': ttype = EQUAL; break;
	case ':': ttype = COLON; break;
	case ',': ttype = COMMA; break;
	case ';': ttype = SEMICOLON; break;
	case '[': ttype = LBRAC; break;
	case ']': ttype = RBRAC; break;
	case '(': ttype = LPAREN; break;
	case ')': ttype = RPAREN; break;
	case '{': ttype = LBRACE; break;
	case '}': ttype = RBRACE; break;
	case '<':
		c = getchar();
		if (c == '>')
			ttype = NOTEQUAL;
		else
		{
			ungetc(c, stdin);
			ttype = LESS;
		}
		break;

	case '>':
		ttype = GREATER;
		break;

	default:
		if (isdigit(c))
		{
			ungetc(c, stdin);
			ttype = scan_number();
		}
		else if (isalpha(c))
		{
			// token is either a keyword or ID
			ungetc(c, stdin);
			ttype = scan_id_or_keyword();
		}
		else if (c == EOF)
			ttype = EOF;
		else
			ttype = ERROR;
		break;
	}    // End Switch
	return ttype;
}