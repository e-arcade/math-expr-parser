#ifndef SRC_TOKEN_H
#define SRC_TOKEN_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEXEME_LENGTH 255
#define MAX_OPERATOR_LENGTH 3
#define FUNCTIONS_SIZE 9

enum token_kind {
	TOKEN_NUMBER,
	TOKEN_FUNCTION,
	TOKEN_VARIABLE,

	// operators
	TOKEN_BIN_PLUS,
	TOKEN_UN_MINUS,
	TOKEN_BIN_MINUS,
	TOKEN_UN_OTHER,
	TOKEN_RSLASH,
	TOKEN_STAR,
	TOKEN_POWER,
	TOKEN_MODULO,
	TOKEN_OPEN_BRACE,
	TOKEN_CLOSE_BRACE,
};

struct token_operator {
	union {
		char value;
		char mod_value[MAX_OPERATOR_LENGTH + 1];
	};
	int priority;
};

struct token {
	union {
		double number;
		char variable;
		struct token_operator operator;
		char func[MAX_LEXEME_LENGTH];
	};
	char lexeme[MAX_LEXEME_LENGTH];
	int func_id;
  	int start_pos;
	enum token_kind kind;
};

struct tokenizer {
	char *expression;
	int exp_size;
	int tokens_size;
	int cur_pos;
	struct token *prev;
};

struct token** tokenize_expression(char *expression, int *tokens_size);
struct token* new_token(char *lexeme, int start_pos, enum token_kind kind);

#endif
