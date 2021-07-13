#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <zlib.h>

#include "gen_c.h"
#include "crc.h"

#define CHUNK_SIZE 4096

void write_header(FILE *dest, config_t *cfg, int file_count)
{
    switch(cfg->lang) {
    case CLANG_HASH:
        write_c_header(dest, cfg, file_count);
        break;
    }
}

void write_body(FILE *dest, FILE *source, config_t *cfg, const char *source_path, int index)
{
    FILE *tmp;
    z_stream strm = {0};
    unsigned char inbuff[CHUNK_SIZE];
    unsigned char outbuff[CHUNK_SIZE];
    int flush;
    unsigned int nbytes;
    crc32_t crc;
    char gzipheader[] = {
            0x1f, 0x8b      // gzip magic number
            , 8             // compress method "defalte"
            , 1             // text data
            , 0, 0, 0, 0    // timestamp is not set
            , 2             // maximum compression flag
            , 255           // unknown OS
        };

    if (cfg->gzip) {
        tmp = tmpfile();

        fwrite(gzipheader, 1, 10, tmp);

        crc32_init(&crc);

        deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED,
                    -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);

        do {
            strm.avail_in = fread(inbuff, 1, CHUNK_SIZE, source);

            if(ferror(source)) {
                deflateEnd(&strm);
                return;
            }

            crc32_update(&crc, inbuff, strm.avail_in);

            flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
            strm.next_in = inbuff;

            do {
                strm.avail_out = CHUNK_SIZE;
                strm.next_out  = outbuff;
                
                deflate(&strm, flush);
                
                nbytes = CHUNK_SIZE - strm.avail_out;

                if(fwrite(outbuff, 1, nbytes, tmp) != nbytes || ferror(tmp)) {
                    deflateEnd(&strm);
                    return;
                }
            } while (strm.avail_out == 0);

        } while (flush != Z_FINISH);

        deflateEnd(&strm);

        fwrite(crc.crca, 1, 4, tmp);

        fseek(tmp, 0L, SEEK_SET);

        source = tmp;
    }

    switch(cfg->lang) {
    case CLANG_HASH:
        write_c_body(dest, source, cfg, source_path, index);
        break;
    }

    if (cfg->gzip) {
        fclose(tmp);
    }
}

void write_footer(FILE *dest, config_t *cfg)
{
    switch(cfg->lang) {
    case CLANG_HASH:
        write_c_footer(dest, cfg);
        break;
    }
}

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    config_t cfg;
    int z = 0, i = xxoh_getopt(argc, argv);
    char wd[1024] = {0};

    xxoh_getconfig(&cfg);

    if (cfg.cwd) {
        getwd(wd);
        chdir(cfg.cwd);
    }

    if (!cfg.nowrap)
        write_header(stdout, &cfg, cfg.stdind ? 1 : argc - i);

    if (cfg.stdind) {
        write_body(stdout, stdin, &cfg, cfg.name, 1);
    } else {
        for (; i < argc; ++i) {
            z++;

            fp = fopen(argv[i], "r");

            if (!fp) {
                fprintf(stderr, "Cannot open \"%s\" %s\n", argv[i], strerror(errno));
                goto invalid_ended;
            }

            write_body(stdout, fp, &cfg, argv[i], z);
            
            fclose(fp);
        }
    }

    if (!cfg.nowrap)
        write_footer(stdout, &cfg);

    return 0;

invalid_ended:
    
    if (cfg.cwd)
        chdir(wd);

    exit(EXIT_FAILURE);
}