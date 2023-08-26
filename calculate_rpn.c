#include "calculate_rpn.h"

static inline double calculate_func_by_id(int func_id, double x)
{
	double result;

	switch (func_id) {
	case 1: result = cos(x); break;
	case 2: result = sin(x); break;
	case 3: result = tan(x); break;
	case 4: result = acos(x); break;
	case 5: result = asin(x); break;
	case 6: result = atan(x); break;
	case 7: result = sqrt(x); break;
	case 8: result = log(x); break;
	case 9: result = log10(x); break;
	
	default:
		break;
	}

	return result;
}

static void calculate_operator(struct list *_list, struct token *operator)
{
	if (operator->kind == TOKEN_UN_MINUS) {
		get_list_tail(_list)->number *= -1;
	} else {
		struct list_node *l = pop_from_list(_list);
		struct token *op_2 = l->token;
		struct token *op_1 = get_list_tail(_list);
		// (+,-,*,/,^,mod)
		switch (operator->kind) {
		case TOKEN_BIN_PLUS:
			op_1->number += op_2->number;
			break;
		case TOKEN_BIN_MINUS:
			op_1->number -= op_2->number;
			break;
		case TOKEN_STAR:
			op_1->number *= op_2->number;
			break;
		case TOKEN_RSLASH:
			op_1->number /= op_2->number;
			break;
		case TOKEN_POWER:
			op_1->number = pow(op_1->number, op_2->number);
			break;
		case TOKEN_MODULO:
			op_1->number = modf(op_1->number, &op_2->number);
			break;

		default:
			break;
		}
		free(l->token);
		free(l);
	}
}

// 2+sin(5) -> 2 5 sin + -> 2 sin5 + -> 2+sin5
double calculate_rpn(struct token **output, int rpn_tokens_size, double *x)
{
	struct list *_list = new_list();
	double result;

	for (int i = 0; i < rpn_tokens_size; i++) {
		struct token *cur = output[i];

		if (cur->kind == TOKEN_NUMBER) {
			push_to_list(_list, new_token(cur->lexeme, cur->start_pos, cur->kind));
		} else if (cur->kind == TOKEN_VARIABLE) {
			struct token *var = new_token(cur->lexeme, cur->start_pos, cur->kind);
			var->number = *x;
			push_to_list(_list, var);
		} else if (cur->kind == TOKEN_FUNCTION) {
			struct token *func_val = get_list_tail(_list);
			if (func_val->variable == 'x') {
				func_val->number = calculate_func_by_id(cur->func_id, *x);
			} else {
				func_val->number = calculate_func_by_id(cur->func_id, func_val->number);
			}
		} else {
			calculate_operator(_list, cur);
		}
	}
	result = get_list_tail(_list)->number;
	free(_list->head->token);
	free(_list->head);
	free(_list);

	return result;
}
