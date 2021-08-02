/*
 * Copyright (c) 2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTIL_DISJOINT_SET_H_
#define _UTIL_DISJOINT_SET_H_

#define DISJOINT_SET_MAX 128

struct disjoint_set {
	int max_nodes;
	int rank[DISJOINT_SET_MAX];
	int parent[DISJOINT_SET_MAX];
};

int disjoint_set_make(struct disjoint_set *set, int max);
int disjoint_set_find(struct disjoint_set *set, int a);
void disjoint_set_union(struct disjoint_set *set, int a, int b);
int disjoint_set_num_roots(struct disjoint_set *set);

#endif /* _UTIL_DISJOINT_SET_H_ */
