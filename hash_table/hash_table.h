#pragma once
#include <stddef.h>

typedef struct ht ht;

typedef struct {
  const char *key;
  void *value;
  ht *_table;
  size_t _index;
} hti;

typedef struct {
  const char *key;
  void *value;
} ht_entry;

struct ht {
  ht_entry *entries;
  size_t capacity;
  size_t length;
};
ht *hash_create(void);
void hash_destroy(ht *table);
void hash_insert(ht *table, ht_entry);
void *hash_get(ht *table, const char *key);
void hash_delete(ht *table, const char *key);
void hash_print(ht *table);
