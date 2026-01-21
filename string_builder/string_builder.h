#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  char *data;
  size_t length;
  size_t size;

} Stringbuilder;

Stringbuilder *create_sb(size_t size);
void destroy_sb(Stringbuilder *sb);
bool append_sb(Stringbuilder *sb, const char *str);
bool insert_sb(Stringbuilder *sb, const char *str, size_t position);
const char *get_string(Stringbuilder *sb);
void clear_sb(Stringbuilder *sb);
bool sb_reserve(Stringbuilder *sb, size_t new_capacity);
void sb_shrink_to_fit(Stringbuilder *sb);
bool sb_append_format(Stringbuilder *sb, const char *format, ...);
