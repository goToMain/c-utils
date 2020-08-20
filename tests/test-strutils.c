#include "test.h"
#include <utils/utils.h>
#include <utils/strutils.h>

int _test_str_sep_do(char *input, char *sep, int toks_len, char *toks[], bool outcome)
{
	int len = 0;
	char *p, *tok, buf[64+1] = {0};

	strncpy(buf, input, 64);
	p = buf;

	while (len < toks_len && (tok = str_sep(&p, sep))) {
		if (strcmp(tok, toks[len]) != 0) {
			if (outcome) {
				mod_printf("strsep failed: "
				"tok: '%s', res: '%s'", tok, p);
			}
			return -1;
		}
		len++;
	}
	return len == toks_len ? 0 : -1;
}

#define _test_sep_pos(i,s,t) if (_test_str_sep_do(i,s,ARRAY_SIZEOF(t),t,true)) break;
#define _test_sep_neg(i,s,t) if (_test_str_sep_do(i,s,ARRAY_SIZEOF(t),t,false) == 0) break;
int test_str_sep()
{
	int retval = -1;
	char *empty = "", *tc01[] = { "1", "2", "3", "4", "5" };

	do {
		_test_sep_pos("1,2,3,4,5", ",", tc01);
		_test_sep_pos(",, 1 ,2  , ,3 ,  4,5  ", ", ", tc01);
		_test_sep_pos("1,,,2 3,,,4 5", ", ", tc01);
		_test_sep_neg("0,1,,,2 3,,,4 5", ", ", tc01);
		_test_sep_neg("1,,,2 3,,,4 5", ",", tc01);
		_test_sep_neg("A,,,2,3,,,4 5", ",", tc01);
		_test_sep_neg("1,,,2,3,,,", ",", tc01);
		retval = 0;
	} while (0);
	return retval;
}

int test_str_sep_count()
{
	if (str_sep_count("1,2,3,4,5", ",") != 5)
		return -1;
	if (str_sep_count("   1 2,  3,,,,4,5   ", ", ") != 5)
		return -1;
	if (str_sep_count("1,  2,,,3  ,4    ,5 ", ", ") != 5)
		return -1;
	if (str_sep_count(",,,,,,  1 2 3 4 5", " ,") != 5)
		return -1;
	if (str_sep_count("1 2 3 4 5", ",") != 1)
		return -1;
	return 0;
}

TEST_DEF(strutils)
{
	TEST_MOD_INIT();

	TEST_MOD_EXEC(test_str_sep());
	TEST_MOD_EXEC(test_str_sep_count());

	TEST_MOD_REPORT();
}
