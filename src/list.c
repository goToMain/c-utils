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
