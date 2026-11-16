#include <stdio.h>
#include <string.h>

#include "cli.h"

int cli_parse_args(int argc, char **argv, enum Command *command, const char **file_name)
{
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        *command = CMD_HELP;
        return 1;
    }

    if (argc == 3 && strcmp(argv[1], "schema") == 0) {
        *command = CMD_SCHEMA;
        *file_name = argv[2];
        return 1;
    }

    if (argc == 3 && strcmp(argv[1], "stats") == 0) {
        *command = CMD_STATS;
        *file_name = argv[2];
        return 1;
    }

    return 0;
}

void cli_print_help(void)
{
    puts("csvstat - CSV table analyzer");
    puts("");
    puts("Usage:");
    puts("  csvstat --help             show this help");
    puts("  csvstat schema <csv-file>  print number of rows, columns and column types");
    puts("  csvstat stats <csv-file>   print statistics for every column");
    puts("");
    puts("Exit codes:");
    puts("  0 - success");
    puts("  1 - file, data or memory error");
    puts("  2 - wrong command line arguments");
}
