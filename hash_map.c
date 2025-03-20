#include "hash_map.h"
#include "string.h"

unsigned int hash(const char *key) {
  unsigned int hash = 0;
  while (*key) {
    hash = (hash * 31) + *key;
    key++;
  }
  return hash % table_size;
}

hash_map_ptr create_hash_map(void) {
  hash_map_ptr map = malloc(sizeof(hash_map_t));
  map->table = malloc(table_size);
  for (int i = 0; i < table_size; i++) {
    map->table[i] = NULL;
  }
  return map;
}

void insert(hash_map_ptr map, const char *key, int value) {
  unsigned int index = hash(key);
  node_ptr new_node = (node_ptr)malloc(sizeof(node_t));
  new_node->key = strdup(key);
  new_node->value = value;
  new_node->next = map->table[index];
  map->table[index] = new_node;
}

int get(hash_map_ptr map, const char *key) {
  unsigned int index = hash(key);
  node_ptr current = map->table[index];
  while (current) {
    if (strcmp(current->key, key) == 0) {
      return current->value;
    }
    current = current->next;
  }
  return -1;  // Key not found
}

void remove_key(hash_map_ptr map, const char *key) {
  unsigned int index = hash(key);
  node_ptr current = map->table[index];
  node_ptr prev = NULL;

  while (current) {
    if (strcmp(current->key, key) == 0) {
      if (prev) {
        prev->next = current->next;
      } else {
        map->table[index] = current->next;
      }
      free(current->key);
      free(current);
      return;
    }
    prev = current;
    current = current->next;
  }
}

void free_hash_map(hash_map_ptr map) {
  for (int i = 0; i < table_size; i++) {
    node_ptr current = map->table[i];
    while (current) {
      node_ptr temp = current;
      current = current->next;
      free(temp->key);
      free(temp);
    }
  }
  free(map);
}
