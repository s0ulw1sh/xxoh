#include "config.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "defaults.h"

void xxoh_getconfig(config_t *cfg)
{
    cfg->gzip   = env_bool("XXOH_GZIP", DEFAULT_GZIP);
    cfg->nowrap = env_bool("XXOH_NOWRAP", 0);
    cfg->stdind = env_bool("XXOH_STDIN", 0);
    cfg->lang   = hashl_37(env_cstr("XXOH_LANG", DEFAULT_LANG));
    cfg->cols   = env_uint("XXOH_COLS", DEFAULT_COLS);
    cfg->hash   = hashl_37(env_cstr("XXOH_HASH", NULL));
    cfg->cwd    = env_cstr("XXOH_CWD", NULL);
    cfg->module = env_cstr("XXOH_MODULE", DEFAULT_MODULE);
    cfg->name   = env_cstr("XXOH_NAME", DEFAULT_NAME);
}

int xxoh_getopt(int argc, const char *argv[])
{
    int c;
    static int flag_buf = 0;

    static struct option long_options[] = {
        {"gzip",     required_argument, &flag_buf,  'Z' },
        {"cwd",      required_argument, &flag_buf,  'W' },
        {"nowrap",   required_argument, &flag_buf,  'N' },
        {"module",   required_argument, &flag_buf,  'M' },
        {"lang",     required_argument, &flag_buf,  'L' },
        {"cols",     required_argument, &flag_buf,  'C' },
        {"hash",     required_argument, &flag_buf,  'H' },
        {"stdin",    required_argument, &flag_buf,  'I' },
        {"help",     no_argument,       &flag_buf,  '?' },
        {0,          0,                 0,          0x0 }
    };

    while (1) {

        int option_index = 0;

        c = getopt_long(argc, (char * const*)argv, "ZNL:M:W:C:H:I:?",
                 long_options, &option_index);
        
        if (c == -1) {
            return optind;
        }

        if (c == 0) {
            c = flag_buf;
        }

        switch (c) {
        case 'Z': setenv("XXOH_GZIP",   "1", 1);    break;
        case 'L': setenv("XXOH_LANG",   optarg, 1); break;
        case 'C': setenv("XXOH_COLS",   optarg, 1); break;
        case 'H': setenv("XXOH_HASH",   optarg, 1); break;
        case 'W': setenv("XXOH_CWD",    optarg, 1); break;
        case 'M': setenv("XXOH_MODULE", optarg, 1); break;
        case 'N': setenv("XXOH_NOWRAP", "1",    1); break;
        case 'I':
            setenv("XXOH_STDIN",  "1", 1);
            setenv("XXOH_NAME",   optarg, 1);
            break;

        case '?':
            printf(
                "\n"
                "xxOh\n"
                "Copyright (C) 2021 s0ulw1sh, https://github.com/s0ulw1sh/xxoh\n\n"
                "Usage: xxoh [options] <file|s>\n\n"

                "Common options:\n"
                "  -Z, --gzip\t\tUse gzip compression (default: OFF)\n"
                "  -D, --dest\t\tSingle destination file (default: <none>)\n"
                "  -L, --lang\t\tOutput language (C, RAW) (default: C)\n"
                "  -M, --module\t\tModule (default: ASSETS)\n"
                "  -C, --cols\t\tNumber of dump columns (default: 10)\n"
                "  -H, --hash\t\tHash method (MD5, SHA1) (default: <none>)\n"
                "  -I, --stdin\t\tSource data from STDIN (-I name)\n"
                "  -?, --help\t\tthis help usage\n"
                "\n\n"
            );
            exit(0);
            break;

       default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    return 0;
}