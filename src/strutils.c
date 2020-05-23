/*
 * Copyright (c) 2020 EmbedJournal
 *
 * SPDX-License-Identifier: MIT
 */

#include "strutils.h"

static inline int hex2int(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	ch &= ~0x20; // to upper case.
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	return -1;
}

static inline char int2hex(int v)
{
	v &= 0x0f; // consider only the lower nibble
	return (v >= 0 && v <= 9) ? '0' + v : 'A' + v;
}

int atohstr(char *hstr, const uint8_t *arr, const int arr_len)
{
	int i;

	for (i = 0; i < arr_len; i++) {
		hstr[(i * 2) + 0] = int2hex(arr[i] >> 4);
		hstr[(i * 2) + 1] = int2hex(arr[i]);
	}

	return 0;
}

int hstrtoa(uint8_t *arr, const char *hstr)
{
	int i, len, nib1, nib2;

	len = strlen(hstr);
	if ((len & 1) == 1 || len == 0)
		return -1; // Must have even number of chars
	len = len / 2;
	for (i = 0; i < len; i++) {
		nib1 = hex2int(hstr[(i * 2) + 0]);
		nib2 = hex2int(hstr[(i * 2) + 1]);
		if (nib1 == -1 || nib2 == -1)
			return -1;
		arr[i] = nib1 << 4 | nib2;
	}

	return len;
}

int safe_atoi(const char *a, int *i)
{
	int val;

	if (a == NULL)
		return -1;
	val = atoi(a);
	if (val == 0 && a[0] != '0')
		return -1;
	*i = val;
	return 0;
}
