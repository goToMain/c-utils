/*
 * Copyright (c) 2019 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _UTILS_LIST_H_
#define _UTILS_LIST_H_

struct node_s {
	struct node_s *next;
	struct node_s *prev;
};

struct list_s {
	struct node_s *head;
	struct node_s *tail;
};

typedef struct node_s node_t;
typedef struct list_s list_t;

void list_init(list_t *list);
void list_append(list_t *list, node_t *node);
void list_appendleft(list_t *list, node_t *node);
int list_pop(list_t *list, node_t **node);
int list_popleft(list_t *list, node_t **node);

#endif /* _UTILS_LIST_H_ */
