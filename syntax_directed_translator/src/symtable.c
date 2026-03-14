#include "symtable.h"
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
// 0 == no match
// 1 == match
int symtable_get(const char *name) {
  for (int i = 0; i < table_size; i++) {
    if (strcmp(table[i].name, name)) {
      return table[i].value;
    }
  }
  exit(1);
} /* exit(1) wenn nicht gefunden */
