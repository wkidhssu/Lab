#ifndef CSVSTAT_REPORT_H
#define CSVSTAT_REPORT_H

#include "schema.h"
#include "stats.h"

void report_print_schema(const struct Schema *schema);
void report_print_stats(const struct Schema *schema, const struct ColumnStats *stats);

#endif