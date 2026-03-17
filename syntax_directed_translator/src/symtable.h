#pragma once
typedef struct {
  char name[64];
  int value;
} Symbol;

void symtable_set(const char *name, int value);
void sym_edit_set(const char *name, int value);
int symtable_get(const char *name); /* exit(1) wenn nicht gefunden */
