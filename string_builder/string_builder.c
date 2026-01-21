#include "string_builder.h"
#include <stdlib.h>

Stringbuilder *create_sb(size_t size) {
  Stringbuilder *sb = malloc(sizeof(Stringbuilder));
  sb->current_pos = 0;
  sb->size = size;
  sb->data = calloc(size, sizeof(char));
  return sb;
}
void destroy_sb(Stringbuilder *sb) {
  // XXX: frage über vim/nvim, wenn ich sb->data als erstes geschrieben hab und
  // im nachhinein das free() einfügen will, wie ist der schnellse weg
  free(sb->data);
  free(sb);
}
