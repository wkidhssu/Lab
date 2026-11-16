#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void check_memory(void *pointer)
{
    if (pointer == NULL) {
        // Сообщение об ошибке — только в stderr, в stdout ничего не пишем.
        fprintf(stderr, "csvstat: out of memory\n");
        exit(CSVSTAT_EXIT_ERROR);
    }
}
