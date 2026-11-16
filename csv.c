#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "csv.h"

char *csv_read_line(FILE *file)
{
    int c = getc(file);  // int, а не char: так можно отличить EOF от обычного символа

    if (c == EOF) {
        return NULL;  // строк больше нет
    }

    // Длина строки заранее неизвестна, поэтому массив растёт по мере чтения:
    // когда место кончается, увеличиваем его в 2 раза через realloc.
    int capacity = 16;  // сколько байт сейчас выделено
    int length = 0;     // сколько символов уже записали
    char *line = malloc((size_t)capacity);
    check_memory(line);

    // Читаем символы до конца строки ('\n') или до конца файла (EOF).
    // Последняя строка файла может не заканчиваться на '\n' — это тоже нормально.
    while (c != '\n' && c != EOF) {
        // length + 1, потому что одно место всегда оставляем под '\0'
        if (length + 1 >= capacity) {
            capacity *= 2;
            line = realloc(line, (size_t)capacity);
            check_memory(line);
        }
        line[length] = (char)c;
        length++;
        c = getc(file);
    }

    line[length] = '\0';  // конец C-строки
    return line;
}

char **csv_split_line(char *line, int *count)
{
    // Число полей заранее неизвестно, поэтому массив указателей тоже растёт.
    int capacity = 4;
    char **fields = malloc((size_t)capacity * sizeof(char *));
    check_memory(fields);

    // Первое поле всегда начинается с начала строки.
    int number = 0;
    fields[number] = line;
    number++;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == CSV_DELIMITER) {
            // Здесь заканчивается текущее поле: ставим конец строки
            // вместо запятой. Теперь предыдущее поле — отдельная C-строка.
            line[i] = '\0';

            if (number == capacity) {
                capacity *= 2;
                fields = realloc(fields, (size_t)capacity * sizeof(char *));
                check_memory(fields);
            }

            // Следующее поле начинается сразу после запятой.
            // Если запятая стояла в самом конце, это будет пустое поле "".
            fields[number] = &line[i + 1];
            number++;
        }
    }

    *count = number;
    return fields;
}
