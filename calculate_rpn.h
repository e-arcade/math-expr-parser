#ifndef SRC_CALCULATE_RPN_H
#define SRC_CALCULATE_RPN_H

#include <math.h>

#include "token.h"
#include "list.h"
#include "dijkstra_rpn.h"

double calculate_rpn(struct token **output, int rpn_tokens_size, double *x);

#endif
