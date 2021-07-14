#include "gzip.h"
#include "crc.h"

#include <zlib.h>

#define CHUNK_SIZE 4096

static char gzipheader[] = {
    0x1f, 0x8b      // gzip magic number
    , 8             // compress method "defalte"
    , 1             // text data
    , 0, 0, 0, 0    // timestamp is not set
    , 2             // maximum compression flag
    , 255           // unknown OS
};

int gzip_file(FILE *dest, FILE *src)
{
    z_stream strm     = {0};
    crc32_t crc       = {0};
    int nbytes, flush = 0;

    unsigned char inbuff[CHUNK_SIZE];
    unsigned char outbuff[CHUNK_SIZE];

    fwrite(gzipheader, 1, 10, dest);

    crc32_init(&crc);

    deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED,
                    -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);

    do {
        strm.avail_in = fread(inbuff, 1, CHUNK_SIZE, src);

        if(ferror(src)) {
            deflateEnd(&strm);
            return -1;
        }

        crc32_update(&crc, inbuff, strm.avail_in);

        flush        = feof(src) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = inbuff;

        do {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out  = outbuff;
            
            deflate(&strm, flush);
            
            nbytes = CHUNK_SIZE - strm.avail_out;

            if(fwrite(outbuff, 1, nbytes, dest) != nbytes || ferror(dest)) {
                deflateEnd(&strm);
                return -1;
            }
        } while (strm.avail_out == 0);

    } while (flush != Z_FINISH);

    deflateEnd(&strm);

    fwrite(crc.crca, 1, 4, dest);

    return 0;
}