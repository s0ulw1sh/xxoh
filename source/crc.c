#include "crc.h"

static uint32_t crc32_for_byte(uint32_t r) {
    for(int j = 0; j < 8; ++j)
        r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
    return r ^ (uint32_t)0xFF000000L;
}

void crc32_init(crc32_t *ctx)
{
    ctx->crc = 0;

    for(uint32_t i = 0; i < 0x100; ++i)
        ctx->table[i] = crc32_for_byte(i);
}

void crc32_update(crc32_t *ctx, const void *data, size_t size) {

    for(size_t i = 0; i < size; ++i)
        ctx->crc = ctx->table[(uint8_t)ctx->crc ^ ((uint8_t*)data)[i]] ^ ctx->crc >> 8;
}
