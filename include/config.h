#ifndef _CONFIG_H_
#define _CONFIG_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

typedef struct config_s config_t;

struct config_s {
    unsigned char  gzip, *name, *cwd, *module;
    unsigned int   lang, cols, hash, wrap;
    int lastpind;
};

int xxoh_getopt(int argc, const char *argv[]);

#endif // _CONFIG_H_