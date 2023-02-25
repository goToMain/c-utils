#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <utils/utils.h>
#include <utils/sockutils.h>
#include <utils/fdutils.h>
#include <utils/bus_server.h>

int g_message_id;
uint8_t g_message_data[1024];
int g_message_data_length;
pthread_mutex_t bus_global_lock;

struct bus_client_data {
	int fd;
	int message_id;
};

int bus_server_work_fn(void *arg)
{
	struct bus_client_data *p = arg;
	int rc = -1;
	ssize_t ret;
	int bus_data_len;
	uint8_t bus_data[1024];

	fcntl_setfl(p->fd, O_NONBLOCK);

	for (;;) {
		ret = read_loop(p->fd, bus_data, sizeof(bus_data));
		if (ret < 0) {
			perror("read failed!");
			rc = -1;
			break;
		}
		if (ret > 0) {
			p->message_id += 1;
			pthread_mutex_lock(&bus_global_lock);
			memcpy(g_message_data, bus_data, ret);
			g_message_data_length = (int)ret;
			g_message_id = p->message_id;
			pthread_mutex_unlock(&bus_global_lock);

		}
		if (g_message_id > p->message_id) {
			pthread_mutex_lock(&bus_global_lock);
			memcpy(bus_data, g_message_data, g_message_data_length);
			bus_data_len = g_message_data_length;
			p->message_id = g_message_id;
			pthread_mutex_unlock(&bus_global_lock);

			ret = write_loop(p->fd, bus_data, bus_data_len);
			if (ret < 0) {
				perror("write failed!");
				rc = -1;
				break;
			}
		}
	}
	close(p->fd);
	free(p);
	return rc;
}

int bus_server_queue_work(bus_server_t *s, int fd)
{
	int i;
	work_t *work = NULL;
	struct bus_client_data *client_data;

	for (i = 0; i < s->max_clients; i++) {
		work = s->work + i;
		if (work->status == WQ_WORK_NEW ||
		    work->status == WQ_WORK_COMPLETE)
			break;
	}

	if (i == s->max_clients)
		return -1;

	client_data = calloc(1, sizeof(struct bus_client_data));
	client_data->fd = fd;

	memset(work, 0, sizeof(work_t));
	work->arg = client_data;
	work->work_fn = bus_server_work_fn;

	workqueue_add_work(&s->wq, work);

	while (READ_ONCE(work->status) != WQ_WORK_IN_PROGRESS);

	return 0;
}

void *bus_server_serve(void *arg)
{
	int rc, fd;
	bus_server_t *s = arg;
	struct sockaddr_un cli_addr;
	socklen_t len;

	for(;;) {
		fd = accept(s->fd, (struct sockaddr *)&cli_addr, &len);
		if (fd < 0 && (errno == EAGAIN || errno == EINTR))
			continue;
		if (fd < 0) {
			perror("accept failed");
			break;
		}
		rc = bus_server_queue_work(s, fd);
		if (rc < 0) {
			printf("client[%d]: workqueue full; closing.\n", fd);
			close(fd);
		}
	}
	return NULL;
}

int bus_server_start(bus_server_t *s, int max_clients, const char *path)
{
	int rc;

	memset(s, 0, sizeof(bus_server_t));

	pthread_mutex_init(&bus_global_lock, NULL);

	rc = workqueue_create(&s->wq, max_clients);
	if (rc < 0) {
		printf("failed to setup workqueue\n");
		return -1;
	}

	s->work = calloc(max_clients, sizeof(work_t));
	if (s->work == NULL) {
		perror("work alloc failed");
		return -1;
	}

	rc = sock_unix_listen(path, max_clients);
	if (rc < 0) {
		perror("sock_unix_listen failed");
		return -1;
	}
	s->fd = rc;
	s->path = strdup(path);
	s->max_clients = max_clients;

	rc = pthread_create(&s->thread, NULL, bus_server_serve, (void *)s);
	if (rc < 0) {
		perror("pthread_create failed");
		return -1;
	}

	return 0;
}

void bus_server_stop(bus_server_t *s)
{
	workqueue_destroy(&s->wq);

	pthread_cancel(s->thread);
	pthread_join(s->thread, NULL);

	pthread_mutex_destroy(&bus_global_lock);

	close(s->fd);
	unlink(s->path);
	free(s->path);
}
