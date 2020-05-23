/*
 * Copyright (c) 2020 EmbedJournal
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _UTIL_STRUTIL_H_
#define _UTIL_STRUTIL_H_

#include <stdint.h>
#include <string.h>

/*
 * atohstr - Array to Hex String
 *
 * Usage:
 *   uint8_t arr[4] = { 0xca, 0xfe, 0xba, 0xbe };
 *   char hstr[16];
 *   if (atohstr(hstr, arr, 4) == 0) {
 *       // hstr = "cafebabe\0";
 *   }
 *
 * Note:
 * The passed char *hstr, has to be 2 * arr_len.
 */
int atohstr(char *hstr, const uint8_t *arr, const int arr_len);

/*
 * hstrtoa - Hex String to Array
 *
 * Usage:
 *   uint8_t arr[4];
 *   const char *hstr = "cafebabe";
 *   if (hstrtoa(arr, hstr) == 0) {
 *       // arr[4] = { 0xca, 0xfe, 0xba, 0xbe };
 *   }
 *
 * Note:
 * uint8_t *arr has to be atleast half of strlen(hstr)
 */
int hstrtoa(uint8_t *arr, const char *hstr);

int safe_atoi(const char *a, int *i);

#endif /* _UTIL_STRUTIL_H_ */