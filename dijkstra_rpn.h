#ifndef SRC_DIJKSTRA_H
#define SRC_DIJKSTRA_H

#include "token.h"
#include "list.h"

struct token **dijkstra_implementation(struct token **tokens, 
										int tokens_size, 
										int *rpn_tokens_size);

#endif
