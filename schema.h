#ifndef CSVSTAT_SCHEMA_H
#define CSVSTAT_SCHEMA_H

#include "types.h"

struct Schema {
    int rows;                // сколько строк данных (заголовок не считается)
    int columns;             // сколько столбцов
    char **names;            // names[i] — имя i-го столбца 
    enum ColumnType *types;  // types[i] — тип i-го столбца
};

/*
  Читает файл file_name и заполняет schema.
  Возвращает 1, если всё хорошо.
  Возвращает 0, если была ошибка (файл не открылся, файл пустой,
  неправильное число полей, значение не число). Сообщение об ошибке уже
  напечатано в stderr, а память внутри schema уже освобождена.
 */
int schema_read(const char *file_name, struct Schema *schema);

void schema_free(struct Schema *schema);

#endif
