#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h> // for sleep if needet

int main(int argc, char **argv) {
  ht *table = hash_create();
  printf("Created hash table with capacity: %zu\n", table->capacity);

  // Test insertions
  printf("\n--- Testing insertions ---\n");

  int *val1 = malloc(sizeof(int));
  *val1 = 42;
  hash_insert(table, (ht_entry){"name", val1});
  int *val2 = malloc(sizeof(int));
  *val2 = 99;
  hash_insert(table, (ht_entry){"age", val2});

  int *val3 = malloc(sizeof(int));
  *val3 = 123;
  hash_insert(table, (ht_entry){"score", val3});
  printf("Inserted 3 entries\n");
  int *val4 = malloc(sizeof(int));
  *val4 = 3489;
  hash_insert(table, (ht_entry){"lebensmittel", val4});
  hash_print(table);
  // Test retrieval
  printf("\n--- Testing retrieval ---\n");
  int *retrieved = (int *)hash_get(table, "name");
  if (retrieved) {
    printf("Retrieved 'name': %d\n", *retrieved);
  }

  retrieved = (int *)hash_get(table, "age");
  if (retrieved) {
    printf("Retrieved 'age': %d\n", *retrieved);
  }

  // Test deletion
  printf("\n--- Testing deletion ---\n");
  hash_delete(table, "age");
  printf("Deleted 'age'\n");
  hash_print(table);

  // Test retrieval after deletion
  printf("\n--- Testing retrieval after deletion ---\n");
  retrieved = (int *)hash_get(table, "age");
  if (retrieved) {
    printf("Retrieved 'age': %d (should not happen!)\n", *retrieved);
  } else {
    printf("'age' not found (correct!)\n");
  }

  // Check if other keys still work after deletion
  retrieved = (int *)hash_get(table, "score");
  if (retrieved) {
    printf("Retrieved 'score': %d (should still work!)\n", *retrieved);
  } else {
    printf("'score' not found (BUG - tombstone broke probing!)\n");
  }

  hash_destroy(table);
  printf("\n--- Hash table destroyed ---\n");
  return 0;
}
