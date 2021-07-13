#ifndef _GEN_SHA1_H_
#define _GEN_SHA1_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

#include <stdint.h>
#include <stddef.h>

#define SHA1_HASH 35636

typedef struct sha1_s sha1_t;

struct sha1_s {
    
    union {
        uint32_t h[5];
        uint8_t  digest[20];
    };

    union {
        uint32_t w[16];
        uint8_t  buffer[64];
    };

    size_t size;
    uint64_t totalSize;
};

void sha1_init(sha1_t *ctx);

void sha1_update(sha1_t *ctx, void *data, size_t length);

void sha1_final(sha1_t *ctx);

#endif // _GEN_SHA1_H_