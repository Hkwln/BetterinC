#include "symtable.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

Symbol table[64];
int table_size = 0;

void symtable_set(const char *name, int value) {
  int i = 0;
  while (*name != '\0') {
    table[table_size].name[i++] = *name++;
  }
  table[table_size].name[i] = '\0';
  table[table_size].value = value;
  table_size++;
}
void sym_edit_set(const char *name, int value) {
  for (int i = 0; i < table_size; i++) {
    if (strcmp(table[i].name, name)) {
      table[i].value = value;
    }
  }
  exit(1);
}
// limit min = no match
// else match
int symtable_get(const char *name) {
  for (int i = 0; i < table_size; i++) {
    if (strcmp(table[i].name, name) == 0) {
      return table[i].value;
    }
  }
  return INT_MIN;
} /* exit(1) wenn nicht gefunden */
