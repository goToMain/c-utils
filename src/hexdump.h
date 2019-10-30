/*
 * Copyright (c) 2019 EmbedJournal
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _HEX_DUMP_UTIL_H_
#define _HEX_DUMP_UTIL_H_

/**
 * Description:
 * 	Dumps an array of bytes in HEX and ASCII formats for
 * 	debugging. `head` is string that is printed before the
 * 	actual bytes are dumped.
 *
 * Example:
 * 	int len;
 * 	uint8_t data[MAX_LEN];
 * 	len = get_data_from_somewhere(data, MAX_LEN);
 * 	hexdump("Data From Somewhere", data, len);
 */
void hexdump(const char *head, const uint8_t *data, size_t len);

#endif
