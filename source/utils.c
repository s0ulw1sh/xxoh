#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h> 

#define tolower(c) (unsigned char)((c) | 0x20)
#define isalpha(c)   (tolower(c) >= 'a' && tolower(c) <= 'z')

unsigned short hashl_37(void *str)
{
    char *p;
    unsigned int    i,
                    len,
                    acm = 2139062143;

    if (str == NULL)
        return 0;

    p   = (char *)str;
    len = strlen(p);

    if (len == 0)
        return 0;

    for (i = 0; i < len; ++i)
        acm = 37 * acm + tolower(*(p + i));

    return (unsigned short) acm & 0x0000FFFF;
}

const char * env_cstr(const char *name, const char *def)
{
    const char *out = getenv(name);
    return out == NULL ? def : out;
}

unsigned char env_bool(const char *name, unsigned char def)
{
    const char *out          = getenv(name);
    unsigned char *p         = (unsigned char*) out;
    unsigned char c = 0, val = 0;
    unsigned int  i          = 0;

    if (out == NULL)
        return def;

    while(*p) {

        switch (tolower(*p) + c + i) {
        case '0': return 0; 
        case '1': return 1; 
        
        case 'n' + 'o' + 1: return 1;
        case 'f' + 'f' + 2: return 0;
        case 'e' + 'u' + 3: return 1;
        case 'e' + 's' + 4: return 0;

        case 'o': // ON | OFF
        case 'f' + 'o' + 1:
        
        case 'f': // FALSE
        case 'a' + 'f' + 1:
        case 'l' + 'a' + 2:
        case 's' + 'l' + 3:
        
        case 't': // TRUE
        case 'r' + 't' + 1:
        case 'u' + 'r' + 2:
            break;
        
        default:
            return 0;
        }

        c = tolower(*p);
        i++;
        p++;
    }

    return 0;
}

unsigned int env_uint(const char *name, unsigned int def)
{
    const char *out         = getenv(name);
    unsigned char *p        = (unsigned char*) out;
    unsigned int val        = 0;

    if (out == NULL)
        return def;

    while(*p) {
        if ('0' <= (*p) && (*p) <= '9') {
            val += *p - '0';
        } else {
            return def;
        }
        p++;

        if (*p)
            val *= 10;
    }

    return val;
}

unsigned int get_basename(char *dest, char *path)
{
    char *s = strrchr(path, '/');
    unsigned int len = strlen(path);
    
    if (!s) {
        memcpy(dest, path, len);
        return len;
    } else {
        len = len - ((s + 1) - path);
        if (len > 0)
            memcpy(dest, s + 1, len);
        
        return len;
    }
}

void to_upper(char *dest, char *source)
{
    for (int i = 0; i < strlen(source); ++i) {
        dest[i] = toupper(source[i]);
    }
}

int to_upper_normalize(char *dest, char *source)
{
    int i, c = 0;
    
    for (i = 0; i < strlen(source); ++i) {
        switch (source[i]) {
        case '.':
        case '-':
        case ' ':
            dest[c++] = '_';
            break;
        default:
            if (isalpha(source[i])) {
                dest[c++] = toupper(source[i]);
            }
        }
    }

    return c;
}