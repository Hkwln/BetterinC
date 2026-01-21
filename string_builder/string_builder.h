#pragma once
#include <stdio.h>

typedef struct {
  char *data;
  size_t current_pos;
  size_t size;

} Stringbuilder;

Stringbuilder *create_sb(size_t size);
void destroy_sb(Stringbuilder *sb);
