#ifndef CSVSTAT_COMMON_H
#define CSVSTAT_COMMON_H

enum CsvstatExitCode {
    CSVSTAT_EXIT_OK = 0,      // всё прошло успешно
    CSVSTAT_EXIT_ERROR = 1,   // ошибка файла, входных данных или памяти
    CSVSTAT_EXIT_BAD_CLI = 2  // неправильные аргументы командной строки
};

/*
  Проверка результата malloc / realloc.
  Если pointer == NULL, значит память выделить не получилось.
  Тогда дальше работать нельзя: печатаем ошибку в stderr
  и сразу завершаем программу с кодом 1 (CSVSTAT_EXIT_ERROR).*/
void check_memory(void *pointer);

#endif
