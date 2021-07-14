#ifndef _GZIP_H_
#define _GZIP_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

#include <stdlib.h>
#include <stdio.h>

int gzip_file(FILE *dest, FILE *src);

#endif // _GZIP_H_