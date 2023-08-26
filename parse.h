#ifndef SRC_PARSE_H
#define SRC_PARSE_H

#include "list.h"

struct parser {
	struct token **tokens;
	struct list *braces;
	int tokens_size;
	int cur_pos;
	char *error;
};

char *parse_expression(struct token **tokens, int tokens_size);

#endif
