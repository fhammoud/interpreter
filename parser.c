#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "parser.h"

struct varNode* variables[100];
int varCount = 0;

//varNode array access functions
struct varNode* getValue(char* element)
{
	int i;
	for (i = 0; i < varCount; i++)
	{
		if (strcmp(variables[i]->name, element) == 0)
			return variables[i];
	}
	return NULL;
}

//parsing functions
void parse_var_section()
{
	int i = 0;
	int c = getToken();

	while (c != SEMICOLON)
	{
		if (c == ID)
		{
			struct varNode* var = (struct varNode*)malloc(sizeof(struct varNode));
			var->name = (char*)malloc(sizeof(char) * MAX_TOKEN_LENGTH);
			strcpy(var->name, token);
			var->value = 0;
			variables[i] = var;
			i++;
		}
		c = getToken();
	}
	varCount = i;
}

struct statementNode* parse_body()
{
	struct statementNode* body;

	getToken();	//LBRACE
	body = parse_stmt_list();
	getToken();	//RBRACE

	return body;
}

struct statementNode* parse_stmt_list()
{
	struct statementNode* st;
	struct statementNode* stl;

	st = parse_stmt();

	int c = getToken();

	if (c == ID || c == PRINT || c == WHILE || c == IF || c == SWITCH)
	{
		ungetToken();
		stl = parse_stmt_list();

		//Since I'm adding an extra NOOPSTMT after the switch statement
		//I opted for a list traversal method for simplicity
		struct statementNode* temp = st;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = stl;

		/*if (st->stmt_type == IFSTMT)
			st->next->next = stl;
		else
			st->next = stl;*/
	}
	else
		ungetToken();
	return st;
}

struct statementNode* parse_stmt()
{
	struct statementNode* st = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	int c = getToken();

	switch (c)
	{
	case ID:
		st = parse_assignment_stmt();
		break;
	case PRINT:
		st = parse_print_stmt();
		break;
	case IF:
		st = parse_if_stmt();
		break;
	case WHILE:
		st = parse_while_stmt();
		break;
	case SWITCH:
		st = parse_switch_stmt();
		break;
	default:
		break;
	}

	getToken();		//semicolon
	return st;
}

struct statementNode* parse_assignment_stmt()
{
	struct statementNode* st = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	int val, c;
	st->stmt_type = ASSIGNSTMT;
	st->assign_stmt = (struct assignmentStatement*)calloc(1, sizeof(struct assignmentStatement));
	st->assign_stmt->lhs = getValue(token);
	getToken();	//equal sign
	c = getToken();
	if (c == NUM)
	{
		st->assign_stmt->op1 = (struct varNode*)calloc(1, sizeof(struct varNode));
		val = atoi(token);
		st->assign_stmt->op1->value = val;
	}
	else
		st->assign_stmt->op1 = getValue(token);

	c = getToken();
	if (c == PLUS || c == MINUS || c == MULT || c == DIV)
	{
		st->assign_stmt->op = c;
		c = getToken();
		if (c == NUM)
		{
			st->assign_stmt->op2 = (struct varNode*)calloc(1, sizeof(struct varNode));
			val = atoi(token);
			st->assign_stmt->op2->value = val;
		}
		else
			st->assign_stmt->op2 = getValue(token);
	}
	else
		ungetToken();

	return st;
}

struct statementNode* parse_print_stmt()
{
	struct statementNode* st = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	st->stmt_type = PRINTSTMT;
	st->print_stmt = (struct printStatement*)calloc(1, sizeof(struct printStatement));
	getToken();
	st->print_stmt->id = getValue(token);
	return st;
}

struct statementNode* parse_if_stmt()
{
	struct statementNode* st = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	st->stmt_type = IFSTMT;
	st->if_stmt = (struct ifStatement*)calloc(1, sizeof(struct ifStatement));

	int c;
	//parse condition
	c = getToken();
	if (c == NUM)
	{
		st->if_stmt->op1 = (struct varNode*)calloc(1, sizeof(struct varNode));
		st->if_stmt->op1->value = atoi(token);
	}
	else
		st->if_stmt->op1 = getValue(token);

	st->if_stmt->relop = getToken();

	c = getToken();
	if (c == NUM)
	{
		st->if_stmt->op2 = (struct varNode*)calloc(1, sizeof(struct varNode));
		st->if_stmt->op2->value = atoi(token);
	}
	else
		st->if_stmt->op2 = getValue(token);

	st->if_stmt->true_branch = parse_body();

	struct statementNode* temp = st->if_stmt->true_branch;
	while (temp->next != NULL)
		temp = temp->next;

	struct statementNode* noop = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	noop->stmt_type = NOOPSTMT;

	temp->next = noop;
	st->if_stmt->false_branch = noop;
	st->next = noop;
	ungetToken();

	return st;
}

struct statementNode* parse_while_stmt()
{
	struct statementNode* st = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	st->stmt_type = IFSTMT;
	st->if_stmt = (struct ifStatement*)calloc(1, sizeof(struct ifStatement));

	int c;
	//parse condition
	c = getToken();
	if (c == NUM)
	{
		st->if_stmt->op1 = (struct varNode*)calloc(1, sizeof(struct varNode));
		st->if_stmt->op1->value = atoi(token);
	}
	else
		st->if_stmt->op1 = getValue(token);

	st->if_stmt->relop = getToken();

	c = getToken();
	if (c == NUM)
	{
		st->if_stmt->op2 = (struct varNode*)calloc(1, sizeof(struct varNode));
		st->if_stmt->op2->value = atoi(token);
	}
	else
		st->if_stmt->op2 = getValue(token);

	st->if_stmt->true_branch = parse_body();

	struct statementNode* gt = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	gt->stmt_type = GOTOSTMT;
	gt->goto_stmt = (struct gotoStatement*)calloc(1, sizeof(struct gotoStatement));
	gt->goto_stmt->target = st;

	struct statementNode* temp = st->if_stmt->true_branch;
	while (temp->next != NULL)
		temp = temp->next;

	struct statementNode* noop = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	noop->stmt_type = NOOPSTMT;

	temp->next = gt;
	st->if_stmt->false_branch = noop;
	st->next = noop;
	ungetToken();

	return st;
}

struct statementNode* parse_switch_stmt()
{
	struct statementNode* st;
	getToken();	//switch key
	struct varNode* key = getValue(token);

	//create NOOPSTMT to put at end of switch statement
	struct statementNode* noop = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	noop->stmt_type = NOOPSTMT;

	//parse switch body
	getToken();	//LBRACE
	st = parse_case_list(key, noop);

	struct statementNode* temp = st;
	while (temp->next != NULL)
		temp = temp->next;

	temp->next = noop;

	getToken();	//RBRACE
	ungetToken();
	return st;
}

struct statementNode* parse_case_list(struct varNode* key, struct statementNode* noop)
{
	struct statementNode* st;
	struct statementNode* stl;

	st = parse_case(key, noop);

	int c = getToken();

	if (c == CASE || c == DEFAULT)
	{
		ungetToken();
		stl = parse_case_list(key, noop);
		st->next->next = stl;
	}
	else
		ungetToken();
	return st;
}

struct statementNode* parse_case(struct varNode* key, struct statementNode* noop)
{
	struct statementNode* st;
	int c = getToken();	//CASE
	if (c == CASE)
	{
		st = (struct statementNode*)calloc(1, sizeof(struct statementNode));
		st->stmt_type = IFSTMT;
		st->if_stmt = (struct ifStatement*)calloc(1, sizeof(struct ifStatement));
		st->if_stmt->op1 = key;
		st->if_stmt->relop = NOTEQUAL;
		getToken();	//get case value
		st->if_stmt->op2 = (struct varNode*)calloc(1, sizeof(struct varNode));
		st->if_stmt->op2->value = atoi(token);
		getToken();	//COLON
		st->if_stmt->false_branch = parse_body();

		struct statementNode* temp = st->if_stmt->false_branch;
		while (temp->next != NULL)
			temp = temp->next;

		struct statementNode* gt = (struct statementNode*)calloc(1, sizeof(struct statementNode));
		gt->stmt_type = GOTOSTMT;
		gt->goto_stmt = (struct gotoStatement*)calloc(1, sizeof(struct gotoStatement));
		gt->goto_stmt->target = noop;
		temp->next = gt;

		struct statementNode* if_noop = (struct statementNode*)calloc(1, sizeof(struct statementNode));
		if_noop->stmt_type = NOOPSTMT;

		st->if_stmt->true_branch = if_noop;
		st->next = if_noop;
	}
	else
		st = parse_default_case();

	return st;
}

struct statementNode* parse_default_case()
{
	getToken();	//COLON
	struct statementNode* st; // = (struct statementNode*)calloc(1, sizeof(struct statementNode));
	st = parse_body();

	return st;
}

struct statementNode* parse_program_and_generate_intermediate_representation()
{
	struct statementNode* program;
	parse_var_section();
	program = parse_body();
	return program;
}