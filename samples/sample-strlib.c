#include <stdio.h>
#include "strlib.h"

GLOBAL_STRING_DEF(my_string, 32);

int main()
{
	str_copy(&my_string, "c", "Hello",  5);
	str_copy(&my_string, "a", " World", 6);
	printf("Contents of string: %s\n", my_string.buf);

	str_printf(&my_string, "c", "key1: %d", 100);
	str_printf(&my_string, "a", "\tkey2: %f", 22.0/7);
	printf("Contents of string: %s\n", my_string.buf);

	return 0;
}
