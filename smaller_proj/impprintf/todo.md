# My implementation of formatters just like printf :)

```c
\* EXERCISE requirements:

- 1. Define a type formatter_fn — a function pointer that takes a va_list
     - and writes a string into a provided buffer.
  2. Create handlers for at least:
  - d → print a decimal int
  - s → print a char
  - string
  - x → print an unsigned int as hex
  3. Store them in a table like: typedef struct { char key; formatter_fn fn; }
     FormatEntry; FormatEntry table[] = { {'d', fmt_int}, {'s', fmt_str}, {'x',
     fmt_hex} };
  4. Write my_format(char
     *buf, size_t buflen, const char*fmt, ...) that walks fmt, and when it sees
     %c, looks up c in the table and calls the right handler.
  5. Test it: my_format(buf, sizeof(buf), "Value: %d hex: %x name: %s", 42,
     255, "Alice");

 \*
```
