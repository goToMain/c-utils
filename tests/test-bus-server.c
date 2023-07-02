#include "test.h"
#include <unistd.h>
#include <string.h>

#include <utils/utils.h>
#include <utils/bus_server.h>
#include <utils/sockutils.h>
#include <utils/fdutils.h>

#define TEST_SERVER_PATH "/tmp/test-utils-bus-server"
#define NUM_CLIENTS 5
#define TEST_MSG "12345678901234567890"
#define TEST_MSG_LEN 20

bus_server_t server;

int bus_write_check(int *fd)
{
	ssize_t ret;
	char buf[128];
	int i, write_fd;

	write_fd = randint(NUM_CLIENTS - 1);
	ret = write(fd[write_fd], TEST_MSG, TEST_MSG_LEN);
	if (ret != TEST_MSG_LEN) {
		mod_printf("write %d failed!", write_fd);
		return -1;
	}

	for (i = 0; i < NUM_CLIENTS; i++) {
		if (i == write_fd)
			continue;
		ret = read(fd[i], buf, sizeof(buf));
		if (ret != TEST_MSG_LEN) {
			mod_printf("read %d failed!", i);
			return -1;
		}
		if (strncmp(buf, TEST_MSG, TEST_MSG_LEN)) {
			mod_printf("msg check %d failed!", i);
			return -1;
		}
	}

	return 0;
}

int test_bus_server()
{
	int i, rc = -1;
	int fd[NUM_CLIENTS];

	for (i = 0; i < NUM_CLIENTS; i++) {
		fd[i] = sock_unix_connect(TEST_SERVER_PATH);
		if (fd[i] < 0) {
			mod_printf("connect %d failed!", i);
			return -1;
		}
	}

	for (i = 0; i < 10; i++) {
		if ((rc = bus_write_check(fd)))
			break;
	}

	for (i = 0; i < NUM_CLIENTS; i++)
		close(fd[i]);

	return rc;
}

TEST_DEF(bus_server)
{
	int rc;
	TEST_MOD_INIT();

	rc = bus_server_start(&server, NUM_CLIENTS, TEST_SERVER_PATH);
	if (rc < 0) {
		mod_printf("Failed to start bus server");
		return;
	}
	mod_printf("bus server started");


	TEST_MOD_EXEC( test_bus_server() );

	bus_server_stop(&server);

	TEST_MOD_REPORT();
}
