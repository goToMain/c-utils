/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _UTILS_LIST_H_
#define _UTILS_LIST_H_

typedef struct node_s node_t;
struct node_s {
	node_t *next;
	node_t *prev;
};

struct list_s {
	node_t *head;
	node_t *tail;
};

typedef struct list_s list_t;

void list_init(list_t *list);
void list_append(list_t *list, node_t *node);
void list_appendleft(list_t *list, node_t *node);
int list_pop(list_t *list, node_t **node);
int list_popleft(list_t *list, node_t **node);

void list_remove_node(list_t *list, node_t *node);
int list_remove_nodes(list_t *list, node_t *start, node_t *end);
void list_insert_node(list_t *list, node_t *after, node_t *new);
int list_insert_nodes(list_t *list, node_t *after, node_t *start, node_t *end);

#endif /* _UTILS_LIST_H_ */
