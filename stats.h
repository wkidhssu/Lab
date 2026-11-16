#ifndef CSVSTAT_STATS_H
#define CSVSTAT_STATS_H

#include "schema.h"
 // для каждого столбца хранятся только "накопители" — структура ниже.

struct ColumnStats {
    // --- общее для INT и DOUBLE ---
    int count;     // сколько значений столбца уже обработали
    int positive;  // 
    int negative;  // 
    double mean;   // среднее арифметическое (считается в самом конце)

    // --- только для столбцов INT ---
    long long int_min;
    long long int_max;
    long long quotient_sum;   // сумма частных  value / rows  
    long long remainder_sum;  // сумма остатков value % rows  
    int even;                 // сколько чётных значений

    // --- только для столбцов DOUBLE ---
    double double_min;
    double double_max;
    double double_sum;        // обычная сумма в double
    int integer;              // сколько значений численно целые 
};
int stats_read(const char *file_name, const struct Schema *schema, struct ColumnStats *stats);

#endif
