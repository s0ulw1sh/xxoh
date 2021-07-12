#ifndef _GEN_C_H_
#define _GEN_C_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

#include "config.h"

#include <stdio.h>

#define CLANG_HASH 28094

void write_c_header(FILE *dest, config_t *cfg, int file_count);

void write_c_body(FILE *dest, FILE *source, config_t *cfg, const char *, int index);

void write_c_footer(FILE *dest, config_t *cfg);

#endif // _GEN_C_H_