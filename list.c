#include "list.h"

struct list *new_list()
{
	struct list *_list = (struct list*)malloc(sizeof(struct list));
	_list->head = NULL;
	_list->size = 0;

	return _list;
}

struct list_node *new_node(struct token *op)
{
	struct list_node *node = (struct list_node*)malloc(sizeof(struct list_node));
	node->next = NULL;
	node->token = op;

	return node;
}

void push_to_list(struct list *_list, struct token *op)
{
	if (_list->head == NULL) {
		_list->head = new_node(op);
	} else {
		struct list_node *curr = _list->head;
		while (curr->next) {
			curr = curr->next;
		}
		curr->next = new_node(op);
	}
	_list->size++;
}

struct list_node *pop_from_list(struct list *_list)
{
	struct list_node *last = NULL;
	if (_list->head != NULL) {
		if (_list->head->next == NULL) {
			last = _list->head;
			_list->head = last->next;
		} else {
			struct list_node *curr = _list->head;
			while (curr->next->next) {
				curr = curr->next;
			}
			last = curr->next;
			curr->next = NULL;
		}
		_list->size--;
	}

	return last;
}

struct token *get_list_tail(struct list *_list)
{
	struct token *op = NULL;
	if (_list->head) {
		struct list_node *curr = _list->head;
		while (curr->next) {
			curr = curr->next;
		}
		op = curr->token;
	}

	return op;
}

void free_list(struct list *_list)
{
	while (_list->head != NULL) {
		free(pop_from_list(_list));
	}
}
