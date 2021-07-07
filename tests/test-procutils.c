#include "test.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <utils/filo.h>
#include <utils/utils.h>
#include <utils/procutils.h>

int test_pid_of(void)
{
	FILE *pfile = fopen("/proc/self/status", "r");
	if (pfile == NULL)
		return -1;

	char filename[200] = { 0 };
	fscanf(pfile, "Name: %199s", filename);
	fclose(pfile);

	int pid = any_pid_of(filename);
	return pid == getpid() ? 0 : -1;
}

TEST_DEF(procutils)
{
	TEST_MOD_INIT();

	TEST_MOD_EXEC( test_pid_of() );

	TEST_MOD_REPORT();
}
