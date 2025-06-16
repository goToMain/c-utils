#ifndef _UTILS_CRC32_H_
#define _UTILS_CRC32_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ZEPHYR__

#include <zephyr/sys/crc.h>

#else

uint16_t crc16_itu_t(uint16_t seed, const uint8_t *src, size_t len);

#endif

uint32_t compute_crc32(uint32_t seed, const uint8_t *buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* _UTILS_CRC32_H_ */