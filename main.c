#include "parser.h"
#include <stdint.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int opt;
    int use_stdin = 0;
    char* dasm_filename = NULL;
    int noexec_flag = 0;

    static struct option long_options[] = {
        {"stdin", no_argument, 0, 's'},
        {"dasm", required_argument, 0, 'd'},
        {"noexec", no_argument, 0, 'n'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 's':
            use_stdin = 1;
            break;
        case 'd':
            dasm_filename = optarg;
            break;
        case 'n':
            noexec_flag = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [--stdin] [--dasm <file>] [--noexec] [<file.lm>]\n", argv[0]);
            fprintf(stderr, "  --stdin    Read code from stdin instead of a file\n");
            fprintf(stderr, "  --dasm     Write disassembly to file\n");
            fprintf(stderr, "  --noexec   Only compile, do not execute\n");
            return 1;
        }
    }

    bool_t execute = !noexec_flag;

    if (use_stdin)
    {
        if (optind < argc)
        {
            fprintf(stderr, "Error: Cannot specify both --stdin and a filename\n");
            return 1;
        }
        parser_stdin();
        parser_start(execute, dasm_filename);
        parser_free();
    }
    else if (optind < argc)
    {
        parser_load(argv[optind]);
        parser_start(execute, dasm_filename);
        parser_free();
    }
    else
    {
        fprintf(stderr, "Usage: %s [--stdin] [--dasm <file>] [--noexec] [<file.lm>]\n", argv[0]);
        fprintf(stderr, "  --stdin    Read code from stdin instead of a file\n");
        fprintf(stderr, "  --dasm     Write disassembly to file\n");
        fprintf(stderr, "  --noexec   Only compile, do not execute\n");
        return 1;
    }

    return 0;
}
