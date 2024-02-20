#include <string.h>
#include <stdlib.h>

#include <utils/pcap_gen.h>
#include <utils/utils.h>

/**
 * See https://wiki.wireshark.org/Development/LibpcapFileFormat for packet
 * strcture documentation.
 */

#define PCAP_MAGIC_NUM 0xa1b2c3d4
#define PCAP_VERSION_MAJOR 2
#define PCAP_VERSION_MINOR 4
#define PCAP_CACHE_SIZE (1 << 12)

PACK(struct pcap_header {
	uint32_t magic_number;
	uint16_t version_major;
	uint16_t version_minor;
	uint32_t this_zone;
	int32_t  sigfigs;
	uint32_t snap_len;
	uint32_t link_type;
});

PACK(struct pcap_record_header {
	uint32_t ts_sec;
	uint32_t ts_usec;
	uint32_t incl_len;
	uint32_t orig_len;
});

pcap_t *pcap_start(char *path, uint32_t max_packet_size, uint32_t link_type)
{
	pcap_t *cap;
	struct pcap_header header;
	int ret = 0;

	cap = malloc(sizeof(*cap));
	if (cap == NULL)
		return NULL;
	cap->cache = calloc(1, PCAP_CACHE_SIZE);
	if (cap->cache == NULL) {
		free(cap);
		return NULL;
	}

	header.magic_number = PCAP_MAGIC_NUM;
	header.version_major = PCAP_VERSION_MAJOR;
	header.version_minor = PCAP_VERSION_MINOR;
	header.this_zone = 0;
	header.sigfigs = 0;
	header.snap_len = max_packet_size;
	header.link_type = link_type;

	cap->offset = 0;
	cap->num_packets = 0;
	cap->file = fopen(path, "wb");
	if(cap->file == NULL) {
		free(cap->cache);
		free(cap);
		return NULL;
	}

	ret = fwrite(&header , sizeof(header), 1, cap->file);
	if (!ret) {
		free(cap->cache);
		free(cap);
		return NULL;
	}
	return cap;
}

static int pcap_flush(pcap_t *cap)
{
	if (fwrite(cap->cache, cap->offset, 1, cap->file)) {
		cap->offset = 0;
		return fflush(cap->file);
	}
	return -1;
}

int pcap_add(pcap_t *cap, uint8_t *capture_data, uint32_t length)
{
	struct pcap_record_header header;
	uint32_t sec, usec;

	if (cap->offset + sizeof(header) + length > PCAP_CACHE_SIZE) {
		if (pcap_flush(cap)) {
			return -1;
		}
	}

	get_time(&sec, &usec);
	header.ts_sec = sec;
	header.ts_usec = usec;
	header.orig_len = header.incl_len = length;

	memcpy((char *)cap->cache + cap->offset, &header, sizeof(header));
	cap->offset += sizeof(header);

	memcpy((char *)cap->cache + cap->offset, capture_data, length);
	cap->offset += length;
	cap->num_packets++;
	return 0;
}

int pcap_stop(pcap_t *cap)
{
	int ret = 0;

	ret = pcap_flush(cap);
	if (ret == 0)
		ret = fclose(cap->file);
	free(cap->cache);
	free(cap);
	return ret;
}
