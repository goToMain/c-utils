/*
 * Copyright (c) 2021 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTILS_WORKQUEUE_H_
#define _UTILS_WORKQUEUE_H_

#include <pthread.h>
#include <utils/event.h>
#include <utils/queue.h>

typedef int (*work_fn_t) (void *);
typedef void (*work_complete_fn_t) (void *work);

enum work_status {
	WQ_WORK_NEW,
	WQ_WORK_QUEUED,
	WQ_WORK_IN_PROGRESS,
	WQ_WORK_COMPLETE,
};

typedef struct {
	queue_node_t node;
	int64_t slice;
	enum work_status status;

	/* Filled by user */
	void *arg;
	work_fn_t work_fn;
	work_complete_fn_t complete_fn;
} work_t;

typedef struct {
	int id;
	int state;
	pthread_t thread;
	event_t event;
	void *wq;
} worker_t;

typedef struct {
	worker_t *workers;
	int num_workers;
	queue_t backlog;
	int backlog_count;
	pthread_mutex_t backlog_lock;
} workqueue_t;

int workqueue_create(workqueue_t *wq, int num_workers);
void workqueue_add_work(workqueue_t *wq, work_t *work);
int workqueue_backlog_count(workqueue_t *wq);
void workqueue_destroy(workqueue_t *wq);

#endif /* _UTILS_WORKQUEUE_H_ */