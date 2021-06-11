/*
 * Copyright (c) 2020-2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _UTILS_STACK_H_
#define _UTILS_STACK_H_

#include <utils/list.h>

typedef struct {
	slist_t list;
} stack_t;

#define stack_node_t snode_t

void stack_init(stack_t *stack);
void stack_push(stack_t *stack, stack_node_t *node);
int stack_pop(stack_t *stack, stack_node_t **node);
int stack_get_top(stack_t *stack, stack_node_t **node);

#endif /* _UTILS_STACK_H_ */
