#include <stdlib.h>
#include <stdio.h>

#include <utils/utils.h>
#include <utils/workqueue.h>

enum worker_state_e {
	WQ_WORKER_STATE_OFFLINE,
	WQ_WORKER_STATE_IDLE,
	WQ_WORKER_STATE_RUNNING,
};

static inline worker_t *get_worker(workqueue_t *wq, int worker_ndx)
{
	return wq->workers + worker_ndx;
}

static inline void wakeup_first_free_worker(workqueue_t *wq)
{
	int i;
	worker_t *w;

	for (i = 0; i < wq->num_workers; i++) {
		w = get_worker(wq, i);
		if (w->state == WQ_WORKER_STATE_IDLE) {
			event_set(&w->event);
			break;
		}
	}
}

static inline work_t *get_backlog(workqueue_t *wq)
{
	int rc;
	queue_node_t *node;

	pthread_mutex_lock(&wq->backlog_lock);
	if ((rc = queue_dequeue(&wq->backlog, &node)) == 0)
		wq->backlog_count--;
	pthread_mutex_unlock(&wq->backlog_lock);

	if (rc != 0)
		return NULL;

	return CONTAINER_OF(node, work_t, node);
}

static inline void put_backlog(workqueue_t *wq, work_t *work)
{
	pthread_mutex_lock(&wq->backlog_lock);

	// TODO: Make this a priority queue on work->slice
	queue_enqueue(&wq->backlog, &work->node);
	wq->backlog_count++;

	pthread_mutex_unlock(&wq->backlog_lock);

	wakeup_first_free_worker(wq);
}

static inline void flush_backlog(workqueue_t *wq)
{
	work_t *work;
	queue_node_t *node;

	pthread_mutex_lock(&wq->backlog_lock);
	while(queue_dequeue(&wq->backlog, &node) == 0) {
		work = CONTAINER_OF(node, work_t, node);
		work->status = WQ_WORK_COMPLETE;
		wq->backlog_count--;
	}
	pthread_mutex_unlock(&wq->backlog_lock);
}

static void *workqueue_factory(void *arg)
{
	int rc;
	int64_t slice;
	worker_t *w = arg;
	work_t *work;
	workqueue_t *wq = w->wq;

	w->state = WQ_WORKER_STATE_IDLE;

	while (event_is_set(&w->event)) {
		w->state = WQ_WORKER_STATE_RUNNING;

		while ((work = get_backlog(wq)) != NULL) {
			work->status = WQ_WORK_IN_PROGRESS;
			slice = usec_now();
			rc = -1;
			if (work->work_fn)
				rc = work->work_fn(work->arg);
			work->slice += usec_since(slice);
			if (rc <= 0) {
				work->status = WQ_WORK_COMPLETE;
				if (work->complete_fn)
					work->complete_fn(work);
			} else {
				put_backlog(wq, work);
			}
		}

		w->state = WQ_WORKER_STATE_IDLE;
	}
	return NULL;
}

int workqueue_create(workqueue_t *wq, int num_workers)
{
	int i;
	worker_t *w;

	wq->workers = calloc(num_workers, sizeof(worker_t));
	if (wq->workers == NULL)
		return -1;

	wq->backlog_count = 0;
	queue_init(&wq->backlog);
	pthread_mutex_init(&wq->backlog_lock, NULL);

	for (i = 0; i < num_workers; i++) {
		w = get_worker(wq, i);
		w->id = i;
		w->wq = wq;
		event_init(&w->event, false, true);
		pthread_create(&w->thread, NULL, workqueue_factory, (void *)w);
	}

	wq->num_workers = num_workers;
	return 0;
}

void workqueue_add_work(workqueue_t *wq, work_t *work)
{
	work->slice = 0;
	work->status = WQ_WORK_QUEUED;
	put_backlog(wq, work);
}

int workqueue_backlog_count(workqueue_t *wq)
{
	return wq->backlog_count;
}

void workqueue_destroy(workqueue_t *wq)
{
	int i;
	worker_t *w;

	flush_backlog(wq);
	pthread_mutex_destroy(&wq->backlog_lock);

	for (i = 0; i < wq->num_workers; i++) {
		w = get_worker(wq, i);
		pthread_cancel(w->thread);
		event_cleanup(&w->event);
	}

	free(wq->workers);
}