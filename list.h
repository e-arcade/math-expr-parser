#ifndef SRC_LIST_H
#define SRC_LIST_H

#include "token.h"

struct list_node {
	struct list_node *next;
	struct token *token;
};

struct list {
	struct list_node *head;
	int size;
};

struct list *new_list();
struct list_node *new_node(struct token *op);
void push_to_list(struct list *_list, struct token *op);
struct list_node *pop_from_list(struct list *_list);
struct token *get_list_tail(struct list *_list);
void free_list(struct list *_list);

#endif
