#ifndef _CRC32_H_
#define _CRC32_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

#include <stdint.h>
#include <stddef.h>

typedef struct crc32_s crc32_t;

struct crc32_s
{
    uint32_t crc;
    uint32_t table[0x100];
};

void crc32_init(crc32_t *ctx);

void crc32_update(crc32_t *ctx, const void *data, size_t size);

#endif // _CRC32_H_