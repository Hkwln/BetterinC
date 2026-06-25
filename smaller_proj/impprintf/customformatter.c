#include "customformatter.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void fmt_int(va_list *args, char *string, size_t *pos, size_t buf_len) {
  int num = va_arg(*args, int);
  bool isNEG = num < 0;

  unsigned int n1 = isNEG ? -num : num;
  char number[50];
  if (n1 == 0) {
    number[0] = '0';
    number[1] = '\0';
  }

  int i = 0;
  for (; n1 != 0;) {
    number[i++] = (n1 % 10) + '0';
    n1 /= 10;
  }
  if (isNEG)
    number[i++] = '-';

  number[i] = '\0';
  for (int t = 0; t < i / 2; t++) {
    number[t] ^= number[i - t - 1];
    number[i - t - 1] ^= number[t];
    number[t] ^= number[i - t - 1];
  }
  // insert:
  strcat(string, number);
  *pos += strlen(number);
}

void fmt_str(va_list *args, char *string, size_t *pos, size_t buf_len) {
  char *str = malloc(sizeof(char) * 5000);
  str = va_arg(*args, char *);
  strcat(string, str);
  *pos += strlen(str);
}

void fmt_hex(va_list *args, char *string, size_t *pos, size_t buf_len) {
  const char hex_digits[] = "0123456789ABSDEF";
  int required_digits = 0;
  char hex_string[50];
  unsigned int str = va_arg(*args, unsigned int);
  unsigned int temp = str;
  while (temp) {
    required_digits++;
    temp >>= 4;
  }
  hex_string[required_digits] = '\0';
  temp = str;
  for (int i = required_digits - 1; i >= 0; i--) {
    hex_string[i] = hex_digits[temp & 0xF];
    temp >>= 4;
  }
  strcat(string, hex_string);
  *pos += strlen(hex_string);
}

void snformat(char *buf, size_t buf_len, const char *fmt, ...) {
  FormatEntry table[] = {{'d', fmt_int}, {'s', fmt_str}, {'x', fmt_hex}};
  va_list args;
  va_start(args, fmt);
  buf[0] = '\0';
  size_t pos = 0;
  formatter_fn fn = NULL;
  for (size_t i = 0; fmt[i] != '\0'; i++) {

    if (fmt[i] == '%') {
      for (size_t b = 0; b < sizeof(table) / sizeof(table[0]); b++) {
        if (fmt[i + 1] == table[b].key) {
          fn = table[b].fn;
          break;
        }
      }
      if (fn) {

        fn(&args, buf, &pos, buf_len);
        i++;
      } else {
        buf[pos++] = fmt[i];
      }

    } else {
      buf[pos++] = fmt[i];
    }
  }
  va_end(args);
  buf[pos] = '\0';
}
void format(const char *fmt, ...) {
  size_t buf_len = 3000;
  char *buf = malloc(buf_len);
  FormatEntry table[] = {
      {'d', fmt_int}, {'s', fmt_str}, {'x', fmt_hex}, {'c', fmt_str}};
  va_list args;
  va_start(args, fmt);
  buf[0] = '\0';
  size_t pos = 0;
  formatter_fn fn = NULL;
  for (size_t i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      for (size_t b = 0; b < sizeof(table) / sizeof(table[0]); b++) {
        if (fmt[i + 1] == table[b].key) {
          fn = table[b].fn;
          break;
        }
      }
      if (fn) {
        fn(&args, buf, &pos, buf_len);
        i++;
      } else {
        buf[pos++] = fmt[i];
      }

    } else {
      buf[pos++] = fmt[i];
    }
    if (pos % 10 && pos >= buf_len - 10) {
      buf_len *= 2;
      buf = realloc(buf, buf_len * 2);
    }
  }
  va_end(args);
  buf[pos] = '\0';
  write(STDOUT_FILENO, buf, pos);
}
