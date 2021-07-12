#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "gen_c.h"

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
    switch(cfg->lang) {
    case CLANG_HASH:
        write_c_body(dest, source, cfg, source_path, index);
        break;
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
    FILE *fp, *fr = NULL;
    size_t n;
    config_t cfg;
    int z = 0, i = xxoh_getopt(argc, argv);
    char wd[1024] = {0};

    xxoh_getconfig(&cfg);

    if (cfg.cwd) {
        getwd(wd);
        chdir(cfg.cwd);
    }

    if (cfg.wrap)
        write_header(stdout, &cfg, argc - i);

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

    if (cfg.wrap)
        write_footer(stdout, &cfg);

    return 0;

invalid_ended:
    
    if (cfg.cwd)
        chdir(wd);

    exit(EXIT_FAILURE);
}