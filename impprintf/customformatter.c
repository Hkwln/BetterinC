
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void (*formatter_fn)(va_list *args, char *string, size_t *pos,
                             size_t buf_len);
typedef struct {
  char key;
  formatter_fn fn;
} FormatEntry;

void fmt_int(va_list *args, char *string, size_t *pos, size_t buf_len) {
  int num = va_arg(*args, int);
  char *number = malloc(10000 * sizeof(int));
  if (num == 0) {
    number[0] = '0';
    number[1] = '\0';
  }
  int i = 0;
  for (; num > 0;) {
    number[i++] = (num % 10) + '0';
    num /= 10;
  }
  number[i] = '\0';
  strcat(string, number);
  free(number);
}

void fmt_str(va_list *args, char *string, size_t *pos, size_t buf_len) {
  char *str = va_arg(*args, char *);
  strcat(string, str);
}

void fmt_hex(va_list *args, char *string, size_t *pos, size_t buf_len) {
  const char hex_digits[] = "0123456789ABSDEF";
  int len = 0;
  unsigned int str = va_arg(*args, unsigned int);
  // TODO: convert unsigned in into hex string:
}

void my_format(char *buf, size_t buf_len, const char *fmt, ...) {
  FormatEntry table[] = {{'d', fmt_int}, {'s', fmt_str}, {'x', fmt_hex}};
  va_list args;
  va_start(args, fmt);

#if 1
  size_t pos = 0;
  formatter_fn fn;
  for (size_t i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      for (size_t b = 0; b < 2; b++) {
        if (fmt[i++] == table[b].key) {
          fn = table[b].fn;
        }
      }
      fn(&args, buf, &pos, buf_len);
      buf[pos++] = fmt[i];

    } else {
      buf[pos++] = fmt[i];
    }
  }
#endif
  va_end(args);
  buf[pos] = '\0';
  printf("%s\n", buf);
  // write(STDOUT_FILENO, buf, buf_len);
}
int main(void) {
  char buf[30000];
  // test:
  my_format(buf, sizeof(buf), "value: %d hex: %x name: %s", 42, 255, "Alice");
  return 0;
}
