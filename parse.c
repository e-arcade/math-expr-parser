#include "parse.h"

static void parse_inner_expression(struct parser *_parser, struct token *_token);
static void parse_token(struct parser *_parser);

// ========================================================== UTILS

static inline struct token *get_current_token(struct parser *_parser)
{
	struct token *cur_token = NULL;
	if (_parser->cur_pos < _parser->tokens_size) {
		cur_token = _parser->tokens[_parser->cur_pos];
	}

	return cur_token;
}

static inline int is_valid_operator(char operator)
{
  	return operator == '-' || operator == '+' || operator == '/' || operator == '*' ||
  	operator == '^' || operator == '(' || operator == ')';
}

// ========================================================== PARSING

static void parse_variable(struct parser *_parser)
{
	struct token *_token = get_current_token(_parser);
	if (_token->variable != 'x') {
		sprintf(_parser->error, "Invalid variable name '%c'.\n", _token->variable);
	}
	_parser->cur_pos++; // skip variable
}

static void parse_function(struct parser *_parser)
{
	struct token *_token = get_current_token(_parser);
	if (_token->func_id == 0) {
		sprintf(_parser->error, "Invalid function name '%s'.\n", _token->lexeme);
	} else {
		_parser->cur_pos++; // skip function name
		struct token *brace = get_current_token(_parser);
		if (brace == NULL || brace->kind != TOKEN_OPEN_BRACE) {
			_parser->error = "Expected '(' token after function name.\n";
			
		} else {
			_parser->cur_pos++; // skip open brace
			push_to_list(_parser->braces, brace);
			parse_inner_expression(_parser, _token);
		}
	}
}

static void parse_open_brace(struct parser *_parser)
{
	struct token *open_brace = get_current_token(_parser);
	push_to_list(_parser->braces, open_brace);
	_parser->cur_pos++; // skip open brace
	struct token *_token = NULL;

	parse_inner_expression(_parser, _token);
}

static void parse_inner_expression(struct parser *_parser, struct token *_token)
{
	while ((_token = get_current_token(_parser)) != NULL && 
			_token->kind != TOKEN_CLOSE_BRACE) {
		parse_token(_parser); // parse entire function expression
		if (_parser->error != NULL) return;
	}
	if (_token == NULL) {
		_parser->error = "Expected ')' token after function argument.\n";
	} else {
		_parser->cur_pos++; // skip close brace
		struct list_node *open_brace = pop_from_list(_parser->braces);
		if (open_brace == NULL) {
			_parser->error = "Can't find matching open brace for ')'.\n";
		}
		free(open_brace);
	}
}

// +-+-+-5 - OK
// (*,/,^,mod)5 - ERROR
// 5(+,-,*,/,^,mod)(-)5 - OK
// 5(+,-,*,/,^,mod)(*,/,^,mod)5 - ERROR
static void parse_operator(struct parser *_parser)
{
	struct token *cur = get_current_token(_parser);
	if (cur->kind != TOKEN_MODULO) {
		if (!is_valid_operator(cur->operator.value)) {
			sprintf(_parser->error, "Invalid operator token '%c'.\n", cur->operator.value);
		} else if (cur->operator.value != '-' && cur->kind == TOKEN_UN_OTHER) {
			sprintf(_parser->error, "Invalid unary operator token '%c'.\n", cur->operator.value);
		} else if (cur->kind == TOKEN_OPEN_BRACE || cur->kind == TOKEN_CLOSE_BRACE) {
			_parser->error = "Incorrect place for brace.\n";
		}
	}
	if (_parser->error == NULL)
		_parser->cur_pos++; // skip operator
}

static void parse_token(struct parser *_parser)
{
	struct token *cur_token = get_current_token(_parser);

	if (cur_token->kind == TOKEN_NUMBER) {
		_parser->cur_pos++;
	} else if (cur_token->kind == TOKEN_VARIABLE) {
		parse_variable(_parser);
	} else if (cur_token->kind == TOKEN_FUNCTION) {
		parse_function(_parser);
	} else if (cur_token->kind == TOKEN_OPEN_BRACE) {
		parse_open_brace(_parser);
	} else {
		parse_operator(_parser);
	}
}

static char *parse_handler(struct parser *_parser)
{
	for (; _parser->cur_pos < _parser->tokens_size;) {
		parse_token(_parser);
		if (_parser->error != NULL) break;
	}

	return _parser->error;
}

static struct parser *new_parser(struct token **tokens, int tokens_size)
{
	struct parser *_parser = (struct parser*)malloc(sizeof(struct parser));
	_parser->tokens = tokens;
	_parser->tokens_size = tokens_size;
	_parser->cur_pos = 0;
	_parser->error = 0;
	_parser->braces = new_list();

	return _parser;
}

char *parse_expression(struct token **tokens, int tokens_size)
{
	struct parser *_parser = new_parser(tokens, tokens_size);
	char *error = parse_handler(_parser);
	free_list(_parser->braces);
	free(_parser->braces);
	free(_parser);

	return error;
}
