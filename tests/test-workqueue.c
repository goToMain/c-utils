#include "test.h"
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <utils/workqueue.h>

struct test_work_data {
	int job_id;
	int runs;
};

int num_complete;
pthread_mutex_t test_lock;

int test_work_runner(void *arg)
{
	struct test_work_data *d = arg;

	usleep(1000);

	if (++d->runs == 5) {
		pthread_mutex_lock(&test_lock);
		num_complete++;
		pthread_mutex_unlock(&test_lock);
		return WORK_DONE;
	}

	return WORK_YIELD;
}

#define NUM_JOBS 20
#define NUM_WORKERS 5

int test_workqueue()
{
	int i;
	workqueue_t wq;
	work_t work[NUM_JOBS] = {0};
	struct test_work_data data[NUM_JOBS] = {0};

	pthread_mutex_init(&test_lock, NULL);

	workqueue_create(&wq, NUM_WORKERS);
	mod_printf("Created %d workers", NUM_WORKERS);

	for (i = 0; i < NUM_JOBS; i++) {
		data[i].job_id = i;
		work[i].work_fn = test_work_runner;
		work[i].arg = &data[i];
		workqueue_add_work(&wq, &work[i]);
	}

	while (num_complete != NUM_JOBS);
	mod_printf("Completed %d jobs", NUM_JOBS);

	workqueue_destroy(&wq);
	pthread_mutex_destroy(&test_lock);
	return 0;
}

TEST_DEF(workqueue)
{
	TEST_MOD_INIT();

	TEST_MOD_EXEC( test_workqueue() );

	TEST_MOD_REPORT();
}
