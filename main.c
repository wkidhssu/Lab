#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "common.h"
#include "output.h"
#include "schema.h"
#include "stats.h"

int main(int argc, char **argv)
{
    enum Command command = CMD_HELP;
    const char *file_name = NULL;

    if (!cli_parse_args(argc, argv, &command, &file_name)) {
        fprintf(stderr, "csvstat: wrong arguments, see 'csvstat --help'\n");
        return CSVSTAT_EXIT_BAD_CLI;
    }

    if (command == CMD_HELP) {
        cli_print_help();
        return CSVSTAT_EXIT_OK;
    }

    struct Schema schema;
    if (!schema_read(file_name, &schema)) {
        return CSVSTAT_EXIT_ERROR;  // сообщение уже напечатано в schema_read
    }

    if (command == CMD_SCHEMA) {
        report_print_schema(&schema);
    } else {
        // Команда stats: нужен массив накопителей — по одному на столбец.
        // Его выделяет main, и main же его освобождает.
        struct ColumnStats *stats = malloc((size_t)schema.columns * sizeof(struct ColumnStats));
        check_memory(stats);

        if (!stats_read(file_name, &schema, stats)) {
            free(stats);
            schema_free(&schema);
            return CSVSTAT_EXIT_ERROR;
        }

        report_print_stats(&schema, stats);
        free(stats);
    }

    schema_free(&schema);
    return CSVSTAT_EXIT_OK;
}
