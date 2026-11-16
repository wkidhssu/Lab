#include "types.h"
static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int is_int(const char *text)
{
    int i = 0;

    if (text[i] == '+' || text[i] == '-') {
        i++;
    }

    if (!is_digit(text[i])) {
        return 0;
    }
    while (is_digit(text[i])) {
        i++;
    }

    return text[i] == '\0';
}

int is_double(const char *text)
{
    int i = 0;

    if (text[i] == '+' || text[i] == '-') {
        i++;
    }

    if (!is_digit(text[i])) {
        return 0;
    }
    while (is_digit(text[i])) {
        i++;
    }

    if (text[i] != '.') {
        return 0;
    }
    i++;

    if (!is_digit(text[i])) {
        return 0;
    }
    while (is_digit(text[i])) {
        i++;
    }

    return text[i] == '\0';
}

enum ColumnType widen_type(enum ColumnType column_type, enum ColumnType value_type)
{
    if (value_type > column_type) {
        return value_type;
    }
    return column_type;
}

const char *type_name(enum ColumnType type)
{
    if (type == TYPE_INT) {
        return "INT";
    }
    if (type == TYPE_DOUBLE) {
        return "DOUBLE";
    }
    return "NULL";
}
