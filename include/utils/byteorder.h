#ifndef __BYTEORDER_H_
#define __BYTEORDER_H_

#include <stdint.h>

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#  if (defined(__BYTE_ORDER__)  && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
     (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN) || \
	 (defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN) || \
	 (defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN) || \
     (defined(__sun) && defined(__SVR4) && defined(_BIG_ENDIAN)) || \
     defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
     defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) || \
     defined(_M_PPC)
#    define __BIG_ENDIAN__
#  elif (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || /* gcc */\
     (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN) /* linux header */ || \
	 (defined(_BYTE_ORDER) && _BYTE_ORDER == _LITTLE_ENDIAN) || \
	 (defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN) /* mingw header */ ||  \
     (defined(__sun) && defined(__SVR4) && defined(_LITTLE_ENDIAN)) || /* solaris */ \
     defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || \
     defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
     defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || /* msvc for intel processors */ \
     defined(_M_ARM) /* msvc code on arm executes in little endian mode */
#    define __LITTLE_ENDIAN__
#  endif
#endif

#if !defined(__LITTLE_ENDIAN__) & !defined(__BIG_ENDIAN__)
#  error "UNKNOWN Platform / endianness. Configure endianness checks for this platform or set explicitly."
#endif

#define __bswap_16(x) ((uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
#define __bswap_24(x) ((uint32_t) ((((x) >> 16) & 0xff) | \
				   (((x)) & 0xff00) | \
				   (((x) & 0xff) << 16)))
#define __bswap_32(x) ((uint32_t) ((((x) >> 24) & 0xff) | \
				   (((x) >> 8) & 0xff00) | \
				   (((x) & 0xff00) << 8) | \
				   (((x) & 0xff) << 24)))
#define __bswap_48(x) ((uint64_t) ((((x) >> 40) & 0xff) | \
				   (((x) >> 24) & 0xff00) | \
				   (((x) >> 8) & 0xff0000) | \
				   (((x) & 0xff0000) << 8) | \
				   (((x) & 0xff00) << 24) | \
				   (((x) & 0xff) << 40)))
#define __bswap_64(x) ((uint64_t) ((((x) >> 56) & 0xff) | \
				   (((x) >> 40) & 0xff00) | \
				   (((x) >> 24) & 0xff0000) | \
				   (((x) >> 8) & 0xff000000) | \
				   (((x) & 0xff000000) << 8) | \
				   (((x) & 0xff0000) << 24) | \
				   (((x) & 0xff00) << 40) | \
				   (((x) & 0xff) << 56)))

#if defined(__LITTLE_ENDIAN__)

#define sys_le16_to_cpu(val) (val)
#define sys_cpu_to_le16(val) (val)
#define sys_le24_to_cpu(val) (val)
#define sys_cpu_to_le24(val) (val)
#define sys_le32_to_cpu(val) (val)
#define sys_cpu_to_le32(val) (val)
#define sys_le48_to_cpu(val) (val)
#define sys_cpu_to_le48(val) (val)
#define sys_le64_to_cpu(val) (val)
#define sys_cpu_to_le64(val) (val)
#define sys_be16_to_cpu(val) __bswap_16(val)
#define sys_cpu_to_be16(val) __bswap_16(val)
#define sys_be24_to_cpu(val) __bswap_24(val)
#define sys_cpu_to_be24(val) __bswap_24(val)
#define sys_be32_to_cpu(val) __bswap_32(val)
#define sys_cpu_to_be32(val) __bswap_32(val)
#define sys_be48_to_cpu(val) __bswap_48(val)
#define sys_cpu_to_be48(val) __bswap_48(val)
#define sys_be64_to_cpu(val) __bswap_64(val)
#define sys_cpu_to_be64(val) __bswap_64(val)

#else

#define sys_le16_to_cpu(val) __bswap_16(val)
#define sys_cpu_to_le16(val) __bswap_16(val)
#define sys_le24_to_cpu(val) __bswap_24(val)
#define sys_cpu_to_le24(val) __bswap_24(val)
#define sys_le32_to_cpu(val) __bswap_32(val)
#define sys_cpu_to_le32(val) __bswap_32(val)
#define sys_le48_to_cpu(val) __bswap_48(val)
#define sys_cpu_to_le48(val) __bswap_48(val)
#define sys_le64_to_cpu(val) __bswap_64(val)
#define sys_cpu_to_le64(val) __bswap_64(val)
#define sys_be16_to_cpu(val) (val)
#define sys_cpu_to_be16(val) (val)
#define sys_be24_to_cpu(val) (val)
#define sys_cpu_to_be24(val) (val)
#define sys_be32_to_cpu(val) (val)
#define sys_cpu_to_be32(val) (val)
#define sys_be48_to_cpu(val) (val)
#define sys_cpu_to_be48(val) (val)
#define sys_be64_to_cpu(val) (val)
#define sys_cpu_to_be64(val) (val)

#endif

#endif /* __BYTEORDER_H_ */
