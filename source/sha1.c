#include "sha1.h"
#include "utils.h"

#include <string.h>

#define W(t)            w[(t) & 0x0F]
#define ROL32(a, n)     (((a) << (n)) | ((a) >> (32 - (n))))
#define CH(x, y, z)     (((x) & (y)) | (~(x) & (z)))
#define PARITY(x, y, z) ((x) ^ (y) ^ (z))
#define MAJ(x, y, z)    (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define htobe32(x) ( \
    (((uint32_t)(x) & 0x000000FFUL) << 24) | \
    (((uint32_t)(x) & 0x0000FF00UL) << 8) | \
    (((uint32_t)(x) & 0x00FF0000UL) >> 8) | \
    (((uint32_t)(x) & 0xFF000000UL) >> 24))

#define betoh32 htobe32

static const uint8_t padding[64] =
{
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint32_t k[4] =
{
    0x5A827999,
    0x6ED9EBA1,
    0x8F1BBCDC,
    0xCA62C1D6
};

static void sha1_process_block(sha1_t *ctx)
{
    unsigned int t;
    uint32_t temp;

    uint32_t a = ctx->h[0];
    uint32_t b = ctx->h[1];
    uint32_t c = ctx->h[2];
    uint32_t d = ctx->h[3];
    uint32_t e = ctx->h[4];

    uint32_t *w = ctx->w;

    for(t = 0; t < 16; t++) {
        w[t] = betoh32(w[t]);
    }

    for(t = 0; t < 80; t++) {
        if(t >= 16)
            W(t) = ROL32(W(t + 13) ^ W(t + 8) ^ W(t + 2) ^ W(t), 1);

        if(t < 20)
            temp = ROL32(a, 5) + CH(b, c, d) + e + W(t) + k[0];
        else if(t < 40)
            temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(t) + k[1];
        else if(t < 60)
            temp = ROL32(a, 5) + MAJ(b, c, d) + e + W(t) + k[2];
        else
            temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(t) + k[3];

        e = d;
        d = c;
        c = ROL32(b, 30);
        b = a;
        a = temp;
    }

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
    ctx->h[4] += e;
}

void sha1_init(sha1_t *ctx)
{
    ctx->h[0] = 0x67452301;
    ctx->h[1] = 0xEFCDAB89;
    ctx->h[2] = 0x98BADCFE;
    ctx->h[3] = 0x10325476;
    ctx->h[4] = 0xC3D2E1F0;

    ctx->size = 0;
    ctx->totalSize = 0;
}

void sha1_update(sha1_t *ctx, void *data, size_t length)
{
    size_t n;

    while(length > 0)
    {
        n = MIN(length, 64 - ctx->size);

        memcpy(ctx->buffer + ctx->size, data, n);

        ctx->size      += n;
        ctx->totalSize += n;
        data                = (uint8_t *) data + n;
        length             -= n;

        if(ctx->size == 64) {
            sha1_process_block(ctx);
            ctx->size = 0;
        }
    }
}

void sha1_final(sha1_t *ctx)
{
    unsigned int i;
    size_t paddingSize;
    uint64_t totalSize;

    totalSize = ctx->totalSize * 8;

    if(ctx->size < 56) {
       paddingSize = 56 - ctx->size;
    } else {
       paddingSize = 64 + 56 - ctx->size;
    }

    sha1_update(ctx, padding, paddingSize);

    ctx->w[14] = htobe32((uint32_t) (totalSize >> 32));
    ctx->w[15] = htobe32((uint32_t) totalSize);

    sha1_process_block(ctx);

    for(i = 0; i < 5; i++) {
       ctx->h[i] = htobe32(ctx->h[i]);
    }

    memset(ctx->digesthex, 0, 41);
    bintohexstr(ctx->digesthex, ctx->digest, 20);
}