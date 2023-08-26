#include "rw.h"
#include "parse.h"
#include "dijkstra_rpn.h"
#include "calculate_rpn.h"

void free_tokens(struct token **tokens, int size)
{
	for (int i = 0; i < size; i++) {
		free(tokens[i]);
	}
	free(tokens);
}

int main()
{
	int tokens_size = 0, rpn_tokens_size = 0;
	
	/* input your expression here */
	char *expression = read_expression("sin(x) * 2");
	struct token **tokens = NULL, **rpn_out = NULL;

	if (expression == NULL) {
		printf("Error ocurred while reading the expression.\n");
	} else {
		tokens = tokenize_expression(expression, &tokens_size);
		char *err = parse_expression(tokens, tokens_size);
		if (err != NULL) {
			printf("%s", err);
			goto end;
		}
		rpn_out = dijkstra_implementation(tokens, tokens_size, &rpn_tokens_size);
		
		/* if x is used in an expression, change it to your value */
		double x = 0;
		double result = calculate_rpn(rpn_out, rpn_tokens_size, &x);
		if (result == INFINITY || result == -INFINITY) {
			printf("Division by zero! Aborted.\n");
			goto end;
		}
		printf("result: %lf\n", result);
	}

end:
	free_tokens(tokens, tokens_size);
	free_tokens(rpn_out, rpn_tokens_size);
	free(expression);

	return 0;
}
