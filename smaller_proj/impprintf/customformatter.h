#pragma once

#include <stdarg.h>
#include <stdlib.h>

typedef void (*formatter_fn)(va_list *args, char *string, size_t *pos,
                             size_t buf_len);
typedef struct {
  char key;
  formatter_fn fn;
} FormatEntry;

void snformat(char *buf, size_t buf_len, const char *fmt, ...);
void format(const char *fmt, ...);
