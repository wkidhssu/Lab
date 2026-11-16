#ifndef CSVSTAT_CSV_H
#define CSVSTAT_CSV_H

#include <stdio.h>

#define CSV_DELIMITER ','

/*
  Читает из файла одну строку (до символа '\n' или до конца файла).
  Возвращает строку БЕЗ символа '\n' в динамической памяти.
  Если файл уже закончился и читать больше нечего — возвращает NULL.
*/
char *csv_read_line(FILE *file);

/*
  Разрезает строку line на поля по запятым.
  В *count записывается число полей.
  Возвращает массив указателей: fields[0], fields[1], ... — начало каждого поля.
   */
 
char **csv_split_line(char *line, int *count);

#endif
