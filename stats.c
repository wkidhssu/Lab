#include <stdio.h>
#include <stdlib.h>

#include "csv.h"
#include "schema.h"
#include "stats.h"
#include "types.h"

static int is_whole_number(double x)
{
    if (x > 1e18 || x < -1e18) {
        return 1;
    }

    long long whole_part = (long long)x;  // дробная часть отбрасывается
    return (double)whole_part == x;
}

static void add_int_value(struct ColumnStats *stats, long long value, long long rows)
{
    // Первое значение сразу становится и минимумом, и максимумом.
    if (stats->count == 0) {
        stats->int_min = value;
        stats->int_max = value;
    }
    if (value < stats->int_min) {
        stats->int_min = value;
    }
    if (value > stats->int_max) {
        stats->int_max = value;
    }

    stats->quotient_sum += value / rows;
    stats->remainder_sum += value % rows;

    if (value > 0) {
        stats->positive++;
    }
    if (value < 0) {
        stats->negative++;
    }
    // Остаток от деления на 2 у чётных чисел равен 0 (и у отрицательных тоже)
    if (value % 2 == 0) {
        stats->even++;
    } 

    stats->count++;
}

static double int_mean(long long quotient_sum, long long remainder_sum, long long rows)
{
    long long q = quotient_sum + remainder_sum / rows;
    long long r = remainder_sum % rows;

    long long small = q % 1024;
    long long big = q - small;

    return (double)big + ((double)small + (double)r / (double)rows);
}

// Добавляет одно значение в статистику столбца DOUBLE.

static void add_double_value(struct ColumnStats *stats, double value)
{
    if (stats->count == 0) {
        stats->double_min = value;
        if (value == 0){
            stats->double_min = 0.0;
        }
        stats->double_max = value;
        if (value == 0){
            stats->double_max = 0.0;
        }
    }
    if (value < stats->double_min) {
        stats->double_min = value;
    }
    if (value > stats->double_max) {
        stats->double_max = value;
    }

    stats->double_sum += value;

    // Ноль (0.0, +0.0, -0.0) не больше и не меньше нуля
    if (value > 0) {
        stats->positive++;
    }
    if (value < 0) {
        stats->negative++;
    }
    if (is_whole_number(value)) {
        stats->integer++;
    }

    stats->count++;
}
int stats_read(const char *file_name, const struct Schema *schema, struct ColumnStats *stats)
{
    // Все накопители начинаем с нуля.
    for (int i = 0; i < schema->columns; i++) {
        struct ColumnStats empty = {0};  // все поля структуры равны 0
        stats[i] = empty;
    }

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "csvstat: cannot open file '%s'\n", file_name);
        return 0;
    }

    char *line = csv_read_line(file);
    free(line);

    long long rows = schema->rows;

    while ((line = csv_read_line(file)) != NULL) {
        int count = 0;
        char **fields = csv_split_line(line, &count);

        for (int i = 0; i < schema->columns; i++) {
            if (schema->types[i] == TYPE_INT) {
                // strtoll — стандартное преобразование строки в long long
                long long value = strtoll(fields[i], NULL, 10);
                add_int_value(&stats[i], value, rows);
            } else if (schema->types[i] == TYPE_DOUBLE) {
                // strtod — стандартное преобразование строки в double.
                double value = strtod(fields[i], NULL);
                add_double_value(&stats[i], value);
            }
        }

        free(fields);
        free(line);
    }

    fclose(file);

    if (rows > 0) {
        for (int i = 0; i < schema->columns; i++) {
            if (schema->types[i] == TYPE_INT) {
                stats[i].mean = int_mean(stats[i].quotient_sum, stats[i].remainder_sum, rows);
            } else if (schema->types[i] == TYPE_DOUBLE) {
                stats[i].mean = stats[i].double_sum / (double)rows;
                if (stats[i].mean == 0){
                    stats[i].mean = 0.0;
                }
            }
        }
    }

    return 1;
}
