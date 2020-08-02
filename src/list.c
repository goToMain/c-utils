/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>

#include <utils/list.h>

void list_init(list_t *list)
{
	list->head = NULL;
	list->tail = NULL;
}

void list_append(list_t *list, node_t *node)
{
	node->prev = list->tail;
	node->next = NULL;
	if (list->tail) {
		list->tail->next = node;
	}
	list->tail = node;
	if (!list->head) {
		list->head = node;
	}
}

void list_appendleft(list_t *list, node_t *node)
{
	node->prev = NULL;
	node->next = list->head;
	if (list->head) {
		list->head->prev = node;
	}
	list->head = node;
	if (!list->tail) {
		list->tail = node;
	}
}

int list_pop(list_t *list, node_t **node)
{
	if (!list->tail) {
		return -1;
	}
	*node = list->tail;
	list->tail = list->tail->prev;
	if (list->tail) {
		list->tail->next = NULL;
	}
	return 0;
}

int list_popleft(list_t *list, node_t **node)
{
	if (!list->head) {
		return -1;
	}
	*node = list->head;
	list->head = list->head->next;
	if (list->head) {
		list->head->prev = NULL;
	}
	return 0;
}

void list_remove_node(list_t *list, node_t *node)
{
	if (node->prev == NULL) {
		/* remove head */
		list->head = node->next;
		if (list->head == NULL)
			list->tail = NULL;
		else
			list->head->prev = NULL;
	}
	else if (node->next == NULL) {
		/* remove tail */
		list->tail = node->prev;
		if (list->tail == NULL)
			list->head = NULL;
		else
			list->tail->next = NULL;
	}
	else {
		/* remove in-between */
		node->next->prev = node->prev;
		node->prev->next = node->next;
	}
}

void list_insert_node(list_t *list, node_t *after, node_t *new)
{
	node_t *next;
	
	if (after == NULL) { /* insert at head */
		next = list->head;
		list->head = new;
	} else {
		next = after->next;
		after->next = new;
	}
	new->prev = after;
	new->next = next;
	next->prev = new;
}

void list_insert_nodes(list_t *list, node_t *after, list_t *nodes)
{
	node_t *next;
	
	if (!nodes->head || !nodes->tail) {
		/* Invalid input */
		return;
	}

	if (list->head == NULL) {
		/* If list is empry, the nodes becomes the list */
		list->head = nodes->head;
		list->tail = nodes->tail;
		return;
	}

	if (after == NULL) {
		/* if 'after' node is not given prepend the nodes to list */
		nodes->tail->next = list->head;
		list->head->prev = nodes->tail;
		list->head = nodes->head;
		return;
	}

	/* insert nodes into list after the 'after' node */
	next = after->next;
	after->next = nodes->head;
	nodes->head->prev = after;
	if (next) {
		next->prev = nodes->tail;
		nodes->tail->next = next;
	}
}
