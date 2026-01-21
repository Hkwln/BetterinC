#include "hash_table.h"
#include <stdio.h> //for printf
#include <stdlib.h>
#include <string.h> //for strcmp
#define STANDART_GRÖẞE 3
// use a tombstone instead of NULL on char*
#define TOMBSTONE (char *)-1

// creates a new hash table:
ht *hash_create(void) {
  ht *table = malloc(sizeof(ht));
  table->capacity = STANDART_GRÖẞE;
  table->entries = calloc(table->capacity, sizeof(ht_entry));
  table->length = 0;
  return table;
}
// destroys the whole hash table
void hash_destroy(ht *table) {
  // iteriere über alle slots
  for (int i = 0; i < table->capacity; i++) {
    // frage: muss ich dann überhaupt dieses if statement haben wenn ich die
    // remaining values auch freen will?
    if (table->entries[i].key != NULL && table->entries[i].key != TOMBSTONE) {

      free((void *)table->entries[i].key);
      free(table->entries[i].value);
      table->entries[i].value = NULL;
      table->entries[i].key = TOMBSTONE;
    }
  }
  free(table->entries);
  free(table);
}
// hash function
unsigned long hash(const char *str) {
  unsigned long hash = 2166136261u;
  while (*str) {
    hash ^= (unsigned char)(*str++);
    hash *= 16777619;
  }
  return hash;
}
// insert newentry to hashtable
void hash_insert(ht *table, ht_entry newentry) {
  if (table->length < table->capacity) {
    size_t index = hash(newentry.key) % table->capacity;
    // if hash is empty, else error handling
    if (table->entries[index].key == NULL ||
        table->entries[index].key == TOMBSTONE) {
      // deep copy of newentry
      char *duplicated_key = strdup(newentry.key);
      table->entries[index].key = duplicated_key;
      table->entries[index].value = newentry.value;
      table->length += 1;

    } else {
      for (int i = 1; i < table->capacity; i++) {
        size_t newindex = (index + i) % table->capacity;
        if (table->entries[newindex].key == NULL ||
            table->entries[newindex].key == TOMBSTONE) {
          //     table->entries[newindex] = newentry;
          table->entries[newindex].key = strdup(newentry.key);
          table->entries[newindex].value = newentry.value;
          table->length++;
          break;
        }
      }
    }
  } else {
    // rezize the array:
    ht_entry *old_entries = table->entries;
    size_t old_capacity = table->capacity;
    table->capacity *= 2;
    table->entries = calloc(table->capacity, sizeof(ht_entry));
    for (int n = 0; n < old_capacity; n++) {
      if (old_entries[n].key == NULL || old_entries[n].key == TOMBSTONE)
        continue;
      size_t newhash = hash(old_entries[n].key) % table->capacity;
      for (int i = 0; i < table->capacity; i++) {
        size_t newnewhash = (newhash + i) % table->capacity;
        if (table->entries[newnewhash].key == NULL ||
            table->entries[newnewhash].key == TOMBSTONE) {
          if (old_entries[n].key == NULL || old_entries[n].key == TOMBSTONE)
            continue;
          table->entries[newnewhash].key = old_entries[n].key;
          table->entries[newnewhash].value = old_entries[n].value;
          break;
        }
      }
      free(old_entries);
      hash_insert(table, newentry);
    }
  }
}
// search for the value given the key returning a pointer to it (the value)
void *hash_get(ht *table, const char *key) {
  size_t index = hash(key) % table->capacity;
  for (int i = 0; i < table->capacity; i++) {
    size_t newindex = (index + i) % table->capacity;
    if (table->entries[newindex].key == NULL) {
      return NULL;
    } else if ((table->entries[newindex].key != NULL &&
                table->entries[newindex].key != TOMBSTONE) &&
               strcmp(table->entries[newindex].key, key) == 0) {
      return table->entries[newindex].value;
    }
  }
  return NULL;
}
// delete the value given the key
void hash_delete(ht *table, const char *key) {
  size_t index = hash(key) % table->capacity;
  for (int i = 0; i < table->capacity; i++) {
    size_t newindex = (index + i) % table->capacity;
    if ((table->entries[newindex].key != NULL &&
         table->entries[newindex].key != TOMBSTONE) &&
        strcmp(table->entries[newindex].key, key) == 0) {
      free(table->entries[newindex].value);
      free((void *)table->entries[newindex].key);
      table->entries[newindex].value = NULL;
      table->entries[newindex].key = TOMBSTONE;
      table->length--;
      break;
    }
  }
}
// printing of the whole hash table
void hash_print(ht *table) {
  printf("So this is what you current hash table look like \n");
  printf("capacity: %zu         usage: %zu \n", table->capacity, table->length);
  for (int i = 0; i < table->capacity; i++) {
    if (table->entries[i].key != NULL && table->entries[i].key != TOMBSTONE) {
      printf("key: %s         value: %p \n", table->entries[i].key,
             table->entries[i].value);
    }
  }
}
