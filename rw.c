#include "rw.h"

char* read_expression(char *expression)
{
	char *_expression = (char*)malloc(sizeof(char));
	size_t idx = 0;

	while (*expression != '\0') {
		if (idx > EXPRESSION_LEN - 1) {
			free(_expression);
			return NULL;
		} else {
			void *ptr = (char*)realloc(_expression, sizeof(char) + idx);
			if (ptr == NULL) {
				return NULL;
			} else {
				_expression = ptr;
			}
		}
		_expression[idx] = *expression++;
		idx++;
	}
	_expression = (char*)realloc(_expression, sizeof(char) + idx);
	_expression[idx] = '\0';

	return _expression;
}
