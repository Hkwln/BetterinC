#include "hash_table.h"
#include <stdio.h> //for printf
#include <stdlib.h>
#include <string.h> //for strcmp
#define STANDART_GRÖẞE 3
// use a tombstone instead of NULL on char*
#define TOMBSTONE (char *)-1

ht *hash_create(void) {
  ht *table = malloc(sizeof(ht));
  table->capacity = STANDART_GRÖẞE;
  table->entries = calloc(table->capacity, sizeof(ht_entry));
  table->length = 0;
  return table;
}
void hash_destroy(ht *table) {
  // iteriere über alle slots
  for (int i = 0; i < table->capacity; i++) {

    if (table->entries[i].key != NULL && table->entries[i].key != TOMBSTONE) {

      free((void *)table->entries[i].key);
      table->entries[i].key = TOMBSTONE;
    }
  }
  free(table->entries);
  free(table);
}
// locale funktion: aktuell temporär
unsigned long hash(const char *str) {
  unsigned long hash = 2166136261u;
  while (*str) {
    hash ^= (unsigned char)(*str++);
    hash *= 16777619;
  }
  return hash;
}
void hash_insert(ht *table, ht_entry newentry) {
  if (table->length < table->capacity) {
    size_t index = hash(newentry.key) % table->capacity;
    // if hash is empty, else error handling
    if (table->entries[index].key == NULL ||
        table->entries[index].key == TOMBSTONE) {
      // deep copy of newentry
      table->entries[index].key = strdup(newentry.key);
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
    // rezize the array with calloc
    ht_entry *old_entries = table->entries;
    size_t old_capacity = table->capacity;
    table->capacity *= 2;
    table->entries = calloc(table->capacity, sizeof(ht_entry));
    for (int n = 0; n < old_capacity; n++) {
      size_t newhash = hash(old_entries[n].key) % table->capacity;
      for (int i = 0; i < table->capacity; i++) {
        size_t newnewhash = (newhash + i) % table->capacity;
        if (table->entries[newnewhash].key == NULL ||
            table->entries[newnewhash].key == TOMBSTONE) {
          // here strdup is not necesarry because table->entries[n].key is
          // already strdup copied string

          if (old_entries[newnewhash].key == NULL ||
              old_entries[newnewhash].key == TOMBSTONE)
            continue;
          table->entries[newnewhash].key = old_entries[n].key;
          table->entries[newnewhash].value = old_entries[n].value;
          // length does not change during rehashing
          break;
        }
      }
    }
    // freeing old memory:
    free(old_entries);
    hash_insert(table, newentry);
  }
}
/* TODO: Was passiert wenn es den hash schon gibt, ich das error handling
 * betrieben habe und jetzt den falschen hash mir hole durch hash_Get?*/
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
/* TODO: und hier genauso was ist wenn ich den falschen key lösche*/
void hash_delete(ht *table, const char *key) {
  size_t index = hash(key) % table->capacity;
  for (int i = 0; i < table->capacity; i++) {
    size_t newindex = (index + i) % table->capacity;
    if ((table->entries[newindex].key != NULL &&
         table->entries[newindex].key != TOMBSTONE) &&
        strcmp(table->entries[newindex].key, key) == 0) {
      free(table->entries[newindex].value);
      table->entries[newindex].value = NULL;
      table->entries[newindex].key = TOMBSTONE;
      table->length--;
      break;
    }
  }
}
/* TODO: finish:
 * print the whole hash table
 * what format?*/
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
