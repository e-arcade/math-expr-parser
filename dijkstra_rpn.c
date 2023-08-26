#include "dijkstra_rpn.h"

// ========================================================== UTILITIES

static inline int is_operator_arithmetic(enum token_kind k)
{
	return k == TOKEN_BIN_PLUS || k == TOKEN_BIN_MINUS  || k == TOKEN_UN_MINUS || 
	k == TOKEN_STAR || k == TOKEN_RSLASH || k == TOKEN_POWER || k == TOKEN_MODULO;
}

static inline void free_popped(struct list_node *popped) {free(popped->token);free(popped);}

// ========================================================== DIJKSTRA

static void dijkstra_priorities(struct token *op, struct token **output, struct list *_list, int *idx)
{
	struct token *tail = get_list_tail(_list);
	if (tail != NULL && !(is_operator_arithmetic(tail->kind) && (op->kind == TOKEN_UN_MINUS))) {
		while (tail != NULL && is_operator_arithmetic(tail->kind) && 
			(tail->operator.priority > op->operator.priority || 
			((tail->operator.priority == op->operator.priority) && 
			op->kind != TOKEN_POWER))) {
			output[(*idx)++] = new_token(tail->lexeme, tail->start_pos, tail->kind);
			free_popped(pop_from_list(_list));
			tail = get_list_tail(_list);
		}
	}
	
	/* push operator O1 to list */
	push_to_list(_list, new_token(op->lexeme, op->start_pos, op->kind));
}

static void dijkstra_close_brace(struct token **output, struct list *_list, int *idx)
{
	struct token *tail = get_list_tail(_list);
	while (tail != NULL && tail->kind != TOKEN_OPEN_BRACE) {
		output[(*idx)++] = new_token(tail->lexeme, tail->start_pos, tail->kind);
		free_popped(pop_from_list(_list));
		tail = get_list_tail(_list);
	}

	/* remove open brace from list */
	free_popped(pop_from_list(_list));
	tail = get_list_tail(_list);
	if (tail != NULL && tail->kind == TOKEN_FUNCTION) {
		output[(*idx)++] = new_token(tail->lexeme, tail->start_pos, tail->kind);
		free_popped(pop_from_list(_list));
	}
}

struct token **dijkstra_implementation(struct token **tokens, int tokens_size, int *rpn_tokens_size)
{
	struct token **output = (struct token**)malloc(sizeof(struct token*) * tokens_size);
	struct list *_list = new_list();
	int idx = 0;

	for (int i = 0; i < tokens_size; i++) {
		struct token *cur = tokens[i];

		if (cur->kind == TOKEN_NUMBER || cur->kind == TOKEN_VARIABLE) {
			output[idx++] = new_token(cur->lexeme, cur->start_pos, cur->kind);
		} else if ((cur->kind == TOKEN_FUNCTION && cur->lexeme) || cur->kind == TOKEN_OPEN_BRACE) {
			push_to_list(_list, new_token(cur->lexeme, cur->start_pos, cur->kind));
		} else if (cur->kind == TOKEN_CLOSE_BRACE) {
			dijkstra_close_brace(output, _list, &idx);
		} else {
			dijkstra_priorities(cur, output, _list, &idx);
		}
	}
	if (_list->head != NULL) {
		struct token *tail = NULL;
		while ((tail = get_list_tail(_list))) {
			output[idx++] = new_token(tail->lexeme, tail->start_pos, tail->kind);
			free_popped(pop_from_list(_list));
		}
	}
	*rpn_tokens_size = idx;
	free(_list);

	return output;
}
