# archive

Here is a list of functions which are redundant/archived,
which where previously used, so if you wanted to know something here it is:)

## dfa.c

### why it has been archived

due to the state_t struct and the state_next function,
which does pretty much the same, the is_friendly function has been heavily
simplified :)

```C
bool is_friendly(const char *digits) {
  bool z = false;
  size_t counter = 0;
  size_t i = 0;
  size_t next_promise = 0;
  while (digits[i] != '\0') {
    if ((counter % 10) == 0 && counter != 0) {
      i = ++next_promise;
      counter = 0;
    }
    counter += digits[i] - '0';
    i++;
  }
  if ((counter % 10) == 0 && counter != 0) {
    z = true;
  }
  return z;
}

```
