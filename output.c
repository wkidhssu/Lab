#include <stdio.h>

#include "output.h"
#include "schema.h"
#include "stats.h"
#include "types.h"

static void print_name(const char *name)
{
    putchar('"');
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] == '"') {
            putchar('"');  
        }
        putchar(name[i]);
    }
    putchar('"');
}

void report_print_schema(const struct Schema *schema)
{
    printf("rows %d\n", schema->rows);
    printf("columns %d\n", schema->columns);

    // Столбцы выводятся в исходном порядке: <номер> "<имя>" <тип>
    for (int i = 0; i < schema->columns; i++) {
        printf("%d ", i);
        print_name(schema->names[i]);
        printf(" %s\n", type_name(schema->types[i]));
    }
}

static void print_int_stats(const struct ColumnStats *stats)
{
    printf("nulls 0\n");
    printf("min %lld\n", stats->int_min);
    printf("max %lld\n", stats->int_max);
    printf("mean %.6f\n", stats->mean);
    printf("positive %d\n", stats->positive);
    printf("negative %d\n", stats->negative);
    printf("even %d\n", stats->even);
}

static void print_double_stats(const struct ColumnStats *stats)
{
    printf("nulls 0\n");
    printf("min %.6f\n", stats->double_min);
    printf("max %.6f\n", stats->double_max);
    printf("mean %.6f\n", stats->mean);
    printf("positive %d\n", stats->positive);
    printf("negative %d\n", stats->negative);
    printf("integer %d\n", stats->integer);
}

void report_print_stats(const struct Schema *schema, const struct ColumnStats *stats)
{
    printf("rows %d\n", schema->rows);
    printf("columns %d\n", schema->columns);

    for (int i = 0; i < schema->columns; i++) {
        printf("\n");  // пустая строка отделяет блоки друг от друга

        printf("column %d\n", i);
        printf("name ");
        print_name(schema->names[i]);
        printf("\n");
        printf("type %s\n", type_name(schema->types[i]));

        if (schema->types[i] == TYPE_INT) {
            print_int_stats(&stats[i]);
        } else if (schema->types[i] == TYPE_DOUBLE) {
            print_double_stats(&stats[i]);
        } else {
            // Столбец NULL (бывает, только если строк данных 0):
            printf("rows %d\n", schema->rows);
        }
    }
}