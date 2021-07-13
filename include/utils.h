#ifndef _UTILS_H_
#define _UTILS_H_ 1

#if _MSC_VER > 1000
#   pragma once
#endif

unsigned short hashl_37(void *str);

const char * env_cstr(const char *name, const char *def);

unsigned int env_uint(const char *name, unsigned int def);

unsigned char env_bool(const char *name, unsigned char def);

unsigned int get_basename(char *dest, char *path);

void to_upper(char *dest, char *source);

int to_upper_normalize(char *dest, char *source);

#endif // _UTILS_H_