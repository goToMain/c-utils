/*
 * Copyright (c) 2024 Siddharth Chandrasekaran <sidcha.dev@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UTILS_PCAP_GEN_H_
#define _UTILS_PCAP_GEN_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	FILE *file;
	size_t offset;
	void *cache;
} pcap_t;

pcap_t *pcap_create(char *path, uint32_t max_packet_size, uint32_t link_type);
int pcap_add_record(pcap_t *cap, uint8_t *capture_data, uint32_t length);
void pcap_dump(pcap_t *cap);

#ifdef __cplusplus
}
#endif

#endif /* _UTILS_PCAP_GEN_H_ */
