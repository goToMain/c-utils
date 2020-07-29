/*
 * Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>

#include <utils/strutils.h>

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
	return (v >= 0 && v <= 9) ? '0' + v : 'A' + (v - 10);
}

int atohstr(char *hstr, const uint8_t *arr, const int arr_len)
{
	int i;

	for (i = 0; i < arr_len; i++) {
		hstr[(i * 2) + 0] = int2hex(arr[i] >> 4);
		hstr[(i * 2) + 1] = int2hex(arr[i]);
	}
	hstr[i * 2] = '\0';

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

char *safe_strncpy(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
    return dest;
}

int trim_suffix(char *str, const char *suffix)
{
	int i, j;

	if (!str || !suffix)
		return -1;

	i = strlen(str);
	j = strlen(suffix);

	if (j > i)
		return -1;

	while (j > 0 && str[i-1] == suffix[j-1]) {
		i--; j--;
	}

	return str[i] = '\0';
}

void rstrip(char *str)
{
	int i;

	i = strlen(str);
	while (str[--i] == ' ')
		str[i] = '\0';
}

void lstrip(char *str)
{
	int i = 0, j = 0;

	while (str[i] && str[i] == ' ')
		i++;

	while (i && str[i]) {
		str[j] = str[i];
		i++; j++;
	}

	if (j != 0)
		str[j] = '\0';
}

void strip(char *str)
{
	lstrip(str);
	rstrip(str);
}

void remove_all(char *str, char c)
{
	int i, j = 0;

	for (i = 0; str[i]; i++) {
		if (str[i] != c)
			str[j++] = str[i];
	}
	str[j] = '\0';
}
