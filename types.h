#ifndef CSVSTAT_TYPES_H
#define CSVSTAT_TYPES_H

 /* TYPE_NULL — у столбца пока не встретилось ни одного значения.
  Он остаётся NULL, только если в таблице 0 строк данных (один заголовок).
 */
enum ColumnType {
    TYPE_NULL,
    TYPE_INT,
    TYPE_DOUBLE
};


int is_int(const char *text);

int is_double(const char *text);

// Возвращает новый тип столбца после того, как в нём встретилось значение value_type.
enum ColumnType widen_type(enum ColumnType column_type, enum ColumnType value_type);

// Название типа для вывода: "NULL", "INT" или "DOUBLE".
const char *type_name(enum ColumnType type);

#endif
