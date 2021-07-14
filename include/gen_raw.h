#ifndef _GEN_RAW_H_
#define _GEN_RAW_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

#include "config.h"

#include <stdio.h>

#define RAW_HASH 15297

void write_raw_body(FILE *dest, FILE *source, config_t *cfg, const char *, int index);

#endif // _GEN_RAW_H_