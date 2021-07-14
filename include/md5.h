#ifndef _MD5_H_
#define _MD5_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

#include <stddef.h>
#include <stdint.h>

#define MD5_HASH 8497

typedef struct md5_s md5_t;

struct md5_s {
    uint64_t size;
    uint32_t buffer[4];
    uint8_t  input[64]; 
    uint8_t  digest[16];
    char     digesthex[33];
};

void md5_init(md5_t *ctx);

void md5_update(md5_t *ctx, void *data, size_t size);

void md5_final(md5_t *ctx);

#endif // _MD5_H_