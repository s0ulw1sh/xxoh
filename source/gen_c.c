#include "gen_c.h"
#include "utils.h"
#include "crc.h"
#include "md5.h"
#include "sha1.h"

#include <stdio.h>
#include <string.h>

#define CHUNK_SIZE 4096

void write_c_header(FILE *dest, config_t *cfg, int file_count)
{
    int i;
    char unamemodule[1024] = {0};

    to_upper_normalize(unamemodule, cfg->module);

    fprintf(dest, "#ifndef _%s_H_\n"
                  "#define _%s_H_ 1\n\n", unamemodule, unamemodule);

    fwrite("#if _MSC_VER > 1000\n#   pragma once\n#endif\n\n", 1, 44, dest);

    if (cfg->gzip)
        fprintf(dest, "#define %s_GZIPED 1\n", unamemodule);

    fprintf(dest, "#define %s_FCOUNT %d\n\n", unamemodule, file_count);
}

void write_c_body(FILE *dest, FILE *source, config_t *cfg, const char *source_path, int index)
{
    int i, b = 0, blocks = 0, z = 0;
    size_t n, c, size;
    char bname[1024]       = {0};
    char bnameupn[1024]    = {0};
    char unamemodule[1024] = {0};
    unsigned char buffer[CHUNK_SIZE];
    crc32_t crc;
    md5_t   md5;
    sha1_t  sha1;

    if (cfg->hash == MD5_HASH)
        md5_init(&md5);

    if (cfg->hash == SHA1_HASH)
        sha1_init(&sha1);

    crc32_init(&crc);

    to_upper_normalize(unamemodule, cfg->module);
    get_basename(bname, source_path);
    to_upper_normalize(bnameupn, bname);

    fprintf(dest, "// %s\n", bname);
    fprintf(dest, "// raw path: %s\n\n", source_path);
    fprintf(dest, "#define %s_%d \"%s\"\n\n", unamemodule, index, bname);
    
    fseek(source, 0L, SEEK_END);

    if (cfg->gzip) {
        fprintf(dest, "#define %s_%d_GZIPED 1\n", unamemodule, index);
        fprintf(dest, "#define %s_GZIPED 1\n\n", bnameupn);
    }

    fprintf(dest, "#define %s_%d_GZIP %d\n", unamemodule, index, cfg->gzip);
    fprintf(dest, "#define %s_GZIP %d\n\n", bnameupn, cfg->gzip);

    size = ftell(source);

    fprintf(dest, "#define %s_%d_SIZE %ld\n", unamemodule, index, size);
    fprintf(dest, "#define %s_SIZE %ld\n\n", bnameupn, size);

    fseek(source, 0L, SEEK_SET);

    fprintf(dest, "#define %s_%d_DATA %s_DATA\n", unamemodule, index, bnameupn);
    
    fprintf(dest, "static unsigned char %s_DATA[] = {\n\t", bnameupn);

    do {
        n = fread(buffer, 1, CHUNK_SIZE, source);

        if (b > 0 && n != 0) {
            fprintf(dest, ", ");
        }

        if (n > 0) {
            crc32_update(&crc, buffer, n);

            if (cfg->hash == MD5_HASH)
                md5_update(&md5, buffer, n);

            if (cfg->hash == SHA1_HASH)
                sha1_update(&sha1, buffer, n);

        }

        for (c = 0; c < n; ++c) {

            if (c > 0 && c < n) {
                fprintf(dest, ", ");
            }

            if (z == cfg->cols) {
                fprintf(dest, "\n\t");
                z = 0;
            }

            fprintf(dest, "0x%2.2x", buffer[c]);
            z++;
        }

        if (n > 0)
            b++;

    } while (n != 0);

    fprintf(dest, "\n};\n\n");

    if (cfg->hash == MD5_HASH) {
        md5_final(&md5);
        fprintf(dest, "#define %s_MD5 \"%s\"\n", bnameupn, md5.digesthex);
    }

    if (cfg->hash == SHA1_HASH) {
        sha1_final(&sha1);
        fprintf(dest, "#define %s_SHA1 \"%s\"\n", bnameupn, sha1.digesthex);
    }

    fprintf(dest, "#define %s_CRC32 %u\n\n", bnameupn, crc.crc);
}

void write_c_footer(FILE *dest, config_t *cfg)
{
    int i;
    char unamemodule[1024] = {0};

    to_upper_normalize(unamemodule, cfg->module);

    fprintf(dest, "#endif // _%s_H_", unamemodule);
}