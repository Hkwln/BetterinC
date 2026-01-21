#include "string_builder.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

Stringbuilder *create_sb(size_t size) {
  Stringbuilder *sb = malloc(sizeof(Stringbuilder));
  if (sb == NULL) {
    printf("error string building failed");
  }
  sb->length = 0;
  sb->size = size;
  sb->data = calloc(size, sizeof(char));
  if (sb->data == NULL) {

    printf("error data allocation failed");
  }
  return sb;
}
void destroy_sb(Stringbuilder *sb) {
  free(sb->data);
  free(sb);
}
bool append_sb(Stringbuilder *sb, const char *str) {
  // testing branch prediction hints 1 means likely
  if (__builtin_expect((strlen(str) <= (sb->size - sb->length - 1)), 1)) {
    // logic for just one char: else a string
    if (strlen(str) == 1) {
      sb->data[sb->length] = str[0];
      sb->length++;
      sb->data[sb->length] = '\0';

    } else {
      strcpy(sb->data + sb->length, str);
      sb->length += strlen(str);
      return true;
    }
  } else {
    size_t needed = sb->length + strlen(str) + 1;
    size_t new_cap = sb->size * 2;
    while (new_cap < needed)
      new_cap *= 2;
    sb_reserve(sb, new_cap);
    append_sb(sb, str);
  }
  return false;
}
bool insert_sb(Stringbuilder *sb, const char *str, size_t position) {
  // check if position is valid:
  if (position < sb->length) {
    // delete all strings right of the position:
    size_t rightlen = sb->length - position;
    char *rightstr = malloc(rightlen + 1);
    memcpy(rightstr, sb->data + position, rightlen);
    rightstr[rightlen] = '\0';
    sb->data[position] = '\0';
    sb->length = position;
    append_sb(sb, str);
    append_sb(sb, rightstr);
    free(rightstr);
    return 1;
  } else {
    return 0;
  }
  return 0;
}
const char *get_string(Stringbuilder *sb) { return (const char *)sb->data; }
void clear_sb(Stringbuilder *sb) {
  sb->data[0] = '\0';
  sb->length = 0;
}
bool sb_reserve(Stringbuilder *sb, size_t new_capacity) {
  size_t old_size = sb->size;
  sb->size = new_capacity;
  const char *old_data = sb->data;
  sb->data = calloc(sb->size, sizeof(char));
  memcpy(sb->data, old_data, old_size);
  free((void *)old_data);
  return true;
}
// shrink the string builder to exactly fit the string size:
void sb_shrink_to_fit(Stringbuilder *sb) {
  if (sb->size > sb->length) {
    sb_reserve(sb, sb->length + 1);
  }
}
/*usage:
     sb_append_format(sb, "User: %s, Age: %d", username, age);
     // Internally: formats the string with sprintf/snprintf, then appends
   result
 */
bool sb_append_format(Stringbuilder *sb, const char *format, ...) {
  va_list args;
  va_start(args, format);
  va_list copyarg;
  va_copy(copyarg, args);
  int size = vsnprintf(NULL, 0, format, copyarg);
  va_end(copyarg);
  size_t remaining_space = sb->size - sb->length;
  if (remaining_space <= size) {
    // Refactor to exactly the size we need:
    sb_reserve(sb, sb->length + size + 1);
  }
  vsnprintf(sb->data + sb->length, sb->size - sb->length, format, args);
  sb->length += size;
  va_end(args);
  return 0;
}
