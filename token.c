#include "token.h"

static char *functions[FUNCTIONS_SIZE] = {
	"cos", "sin", "tan", "acos", "asin", "atan", "sqrt", "ln", "log"
};

// ========================================================== UTILS

static inline char get_current_symbol(struct tokenizer *_tokenizer)
{
	return _tokenizer->expression[_tokenizer->cur_pos];
}

static inline int get_operator_kind(char operator)
{
	int kind = -1;

	switch (operator) {
	case '+':
		kind = TOKEN_BIN_PLUS;
		break;
	case '-':
		kind = TOKEN_BIN_MINUS;
		break;
	case '*':
		kind = TOKEN_STAR;
		break;
	case '/':
		kind = TOKEN_RSLASH;
		break;
	case '^':
		kind = TOKEN_POWER;
		break;
	case '(':
		kind = TOKEN_OPEN_BRACE;
		break;
	case ')':
		kind = TOKEN_CLOSE_BRACE;
		break;
	
	default:
		kind = TOKEN_MODULO;
		break;
	}

	return kind;
}

static inline int get_operator_priority(enum token_kind kind)
{
	int priority = 0;

	switch (kind) {
	case TOKEN_BIN_PLUS:
	case TOKEN_UN_MINUS:
	case TOKEN_BIN_MINUS:
	case TOKEN_UN_OTHER:
		priority = 1;
		break;
	case TOKEN_STAR:
	case TOKEN_RSLASH:
		priority = 2;
		break;
	case TOKEN_POWER:
	case TOKEN_MODULO:
		priority = 3;
		break;
	case TOKEN_OPEN_BRACE:
	case TOKEN_CLOSE_BRACE:
		priority = 4;
		break;
	
	default:
		break;
	}

	return priority;
}

static inline int get_function_id(char *func_lexeme)
{
	int id = -1;
	for (int i = 0; i < FUNCTIONS_SIZE; i++) {
		if (!strcmp(functions[i], func_lexeme)) {
			id = i;
			break;
		}
	}

	return id + 1;
}

static inline int is_operator_unary(struct tokenizer *_tokenizer)
{
	int result = 0;

	if (_tokenizer->prev) {
		result = (_tokenizer->prev->kind != TOKEN_CLOSE_BRACE &&
				  _tokenizer->prev->kind != TOKEN_NUMBER &&
				  _tokenizer->prev->kind != TOKEN_VARIABLE &&
				  _tokenizer->prev->kind != TOKEN_FUNCTION);
	} else {
		result = 1;
	}

	return result;
}

// ========================================================== TOKENIZING

struct token* new_token(char *lexeme, int start_pos, enum token_kind kind)
{
	struct token *_token = (struct token*)malloc(sizeof(struct token));

	switch (kind) {
	case TOKEN_NUMBER:
		_token->number = strtod(lexeme, NULL);
		break;
	case TOKEN_VARIABLE:
		_token->variable = lexeme[0];
		break;
	case TOKEN_FUNCTION:
		strcpy(_token->func, lexeme);
		_token->func_id = get_function_id(lexeme);
		break;
	case TOKEN_MODULO:
		strcpy(_token->operator.mod_value, lexeme);
		_token->operator.priority = get_operator_priority(kind);
		break;

	default:
		_token->operator.value = lexeme[0];
		_token->operator.priority = get_operator_priority(kind);
		break;
	}
	_token->kind = kind;
	strcpy(_token->lexeme, lexeme);
	_token->start_pos = start_pos;

	return _token;
}

static struct token* extract_number(struct tokenizer *_tokenizer)
{
	struct token *token = NULL;
	char number[MAX_LEXEME_LENGTH + 1] = {0};
	char symbol;
	int idx = 0, start_pos = _tokenizer->cur_pos;

	while (isdigit(symbol = get_current_symbol(_tokenizer)) &&
		((symbol = get_current_symbol(_tokenizer)) != '\0')) {
		number[idx++] = symbol;
		_tokenizer->cur_pos++;
	}
	// float number handler
	if ((symbol = get_current_symbol(_tokenizer)) == '.') {
		number[idx++] = symbol;
		_tokenizer->cur_pos++;

		while ((symbol = get_current_symbol(_tokenizer) != '\0' ||
			isdigit(symbol))) {
			number[idx++] = symbol;
			_tokenizer->cur_pos++;
		}
	}

	token = new_token(number, start_pos, TOKEN_NUMBER);

	return token;
}

static struct token* extract_word(struct tokenizer *_tokenizer)
{
	struct token *token = NULL;
	char word[MAX_LEXEME_LENGTH + 1] = {0};
	char symbol;
	int idx = 0, start_pos = _tokenizer->cur_pos;

	while (isalpha(symbol = get_current_symbol(_tokenizer))) {
		word[idx++] = symbol;
		_tokenizer->cur_pos++;
	}
	
	if (!strcmp(word, "x")) {
		token = new_token(word, start_pos, TOKEN_VARIABLE);
	} else if (!strcmp(word, "mod")) {
		token = new_token(word, start_pos, TOKEN_MODULO);
	} else {
		token = new_token(word, start_pos, TOKEN_FUNCTION);
	}

	return token;
}

static struct token** extract_tokens(struct tokenizer *_tokenizer)
{
	struct token **tokens = (struct token**)malloc(sizeof(struct token*) * _tokenizer->exp_size);
	int token_idx = 0;
	char symbol;
	while ((symbol = get_current_symbol(_tokenizer)) != '\0') {
		struct token *token = NULL;
		if (isdigit(symbol)) {
			token = extract_number(_tokenizer);
		} else if (isalpha(symbol)) {
			token = extract_word(_tokenizer);
		} else if (isspace(symbol)) {
			_tokenizer->cur_pos++;
			continue;
		} else {
			if (symbol == '+' && is_operator_unary(_tokenizer)) {
				_tokenizer->cur_pos++;
				continue;
			} else {
				enum token_kind kind = get_operator_kind(symbol);
				if (is_operator_unary(_tokenizer)) {
					if (symbol == '-') {
						kind = TOKEN_UN_MINUS;
					} else if (symbol != '(' && symbol != ')') {
						kind = TOKEN_UN_OTHER;
					}
				}
				char operator[MAX_OPERATOR_LENGTH + 1] = {symbol};
				token = new_token(operator, _tokenizer->cur_pos, kind);
				_tokenizer->cur_pos++;
			}
		}
		tokens[token_idx++] = token;
		_tokenizer->prev = token;
	}
	_tokenizer->tokens_size = token_idx;

	return tokens;
}

static struct tokenizer* new_tokenizer(char *expression, size_t exp_size)
{
	struct tokenizer *new_tokenizer = (struct tokenizer*)malloc(sizeof(struct tokenizer));
	new_tokenizer->expression = expression; // must not be freed
	new_tokenizer->exp_size = exp_size;
	new_tokenizer->tokens_size = 0;
	new_tokenizer->cur_pos = 0;
	new_tokenizer->prev = NULL;

	return new_tokenizer;
}

struct token** tokenize_expression(char *expression, int *tokens_size)
{
	size_t exp_size = strlen(expression);
	struct tokenizer *_tokenizer = new_tokenizer(expression, exp_size);
	struct token **tokens = extract_tokens(_tokenizer);
	*tokens_size = _tokenizer->tokens_size;
	free(_tokenizer);

	return tokens;
}
