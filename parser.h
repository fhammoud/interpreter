#pragma once

/*
* parser.h
*/

struct statementNode* parse_body();
struct statementNode* parse_stmt_list();
struct statementNode* parse_stmt();
struct statementNode* parse_assignment_stmt();
struct statementNode* parse_print_stmt();
struct statementNode* parse_if_stmt();
struct statementNode* parse_while_stmt();
struct statementNode* parse_switch_stmt();
struct statementNode* parse_case_list(struct varNode* key, struct statementNode* noop);
struct statementNode* parse_case(struct varNode* key, struct statementNode* noop);
struct statementNode* parse_default_case();
struct statementNode* parse_program_and_generate_intermediate_representation();