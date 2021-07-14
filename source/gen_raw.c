#include "gen_raw.h"

#define CHUNK_SIZE 4096

void write_raw_body(FILE *dest, FILE *source, config_t *cfg, const char *, int index)
{
    unsigned char buffer[CHUNK_SIZE];
    size_t n, c;
    int z = 0, b = 0;

    do {
        n = fread(buffer, 1, CHUNK_SIZE, source);

        if (b > 0 && n != 0) {
            fprintf(dest, ", ");
        }

        for (c = 0; c < n; ++c) {

            if (c > 0 && c < n) {
                fprintf(dest, ", ");
            }

            if (z == cfg->cols) {
                fprintf(dest, "\n");
                z = 0;
            }

            fprintf(dest, "0x%2.2x", buffer[c]);
            z++;
        }

        if (n > 0)
            b++;

    } while (n != 0);

}