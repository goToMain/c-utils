#ifndef _UTILS_CRC32_H_
#define _UTILS_CRC32_H_

#include <stdint.h>
#include <stddef.h>

uint32_t compute_crc32(uint32_t seed, const uint8_t *buffer, size_t length);

#endif /* _UTILS_CRC32_H_ */