#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "csv.h"
#include "schema.h"
#include "types.h"

// Делает копию строки в динамической памяти
static char *copy_string(const char *text)
{
    size_t length = strlen(text);
    char *copy = malloc(length + 1);  // +1 под символ конца строки '\0'
    check_memory(copy);
    strcpy(copy, text);
    return copy;
}

static void read_header(char *line, struct Schema *schema)
{
    int count = 0;
    char **fields = csv_split_line(line, &count);

    schema->columns = count;

    schema->names = malloc((size_t)count * sizeof(char *));
    check_memory(schema->names);

    schema->types = malloc((size_t)count * sizeof(enum ColumnType));
    check_memory(schema->types);

    for (int i = 0; i < count; i++) {
        // fields[i] указывает ВНУТРЬ line, а line скоро освободят.
        // Поэтому имя копируем в свою собственную память.
        schema->names[i] = copy_string(fields[i]);

        // Пока не видели ни одного значения — тип столбца NULL.
        schema->types[i] = TYPE_NULL;
    }

    free(fields);
}

/*
 * Разбирает одну строку данных и уточняет типы столбцов.
 * Возвращает 1, если строка правильная, и 0, если в ней ошибка.
 */
static int read_data_row(char *line, struct Schema *schema)
{
    int count = 0;
    char **fields = csv_split_line(line, &count);

    // В каждой строке данных должно быть ровно столько полей,
    if (count != schema->columns) {
        fprintf(stderr, "csvstat: data row %d has %d fields, but header has %d columns\n",
                schema->rows + 1, count, schema->columns);
        free(fields);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        enum ColumnType value_type;

        if (is_int(fields[i])) {
            value_type = TYPE_INT;
        } else if (is_double(fields[i])) {
            value_type = TYPE_DOUBLE;
        } else {
            fprintf(stderr, "csvstat: data row %d, column %d: '%s' is not a number\n",
                    schema->rows + 1, i, fields[i]);
            free(fields);
            return 0;
        }

        schema->types[i] = widen_type(schema->types[i], value_type);
    }

    free(fields);
    return 1;
}

int schema_read(const char *file_name, struct Schema *schema)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "csvstat: cannot open file '%s'\n", file_name);
        return 0;
    }

    char *line = csv_read_line(file);
    if (line == NULL) {
        fprintf(stderr, "csvstat: file '%s' is empty\n", file_name);
        fclose(file);
        return 0;
    }
    read_header(line, schema);
    free(line);

    schema->rows = 0;
    while ((line = csv_read_line(file)) != NULL) {
        int ok = read_data_row(line, schema);
        free(line);  // строка больше не нужна — сразу освобождаем

        if (!ok) {
            fclose(file);
            schema_free(schema);
            return 0;
        }
        schema->rows++;
    }

    fclose(file);
    return 1;
}

void schema_free(struct Schema *schema)
{
    // Сначала освобождаем каждое имя, потом сам массив указателей на имена.
    // (Если освободить массив первым, мы потеряем адреса имён — будет утечка.)
    for (int i = 0; i < schema->columns; i++) {
        free(schema->names[i]);
    }
    free(schema->names);
    free(schema->types);

    // Обнуляем, чтобы случайно не использовать освобождённую память.
    schema->names = NULL;
    schema->types = NULL;
    schema->columns = 0;
    schema->rows = 0;
}
