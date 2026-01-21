// counts the number of worlds that has been used in this specific document
// using a hash table
//
#include "hash_table.h"
#include <stdio.h>
int count(ht *ht, char *filename, char *word) {
  FILE *file;
  file = fopen(filename, "w");
  while (fgets(line, sizeof(line), file)) {
  }
  fclose(file);
  return 130;
}
int main(int argc, const char **argv) {
  ht *wordcount = hash_create();
  printf("there are : %i the char table in hash_table.c",
         count(wordcount, "hash_table.c", "table"));
  return 0;
}
