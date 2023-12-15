#ifndef _UTILS_CRC32_H_
#define _UTILS_CRC32_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t compute_crc32(uint32_t seed, const uint8_t *buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* _UTILS_CRC32_H_ */