#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#define table_size 100

typedef struct node_t {
  char *key;
  int value;
  struct node_t *next;
} node_t, *node_ptr;

typedef struct hash_map_t {
  node_ptr *table;
} hash_map_t, *hash_map_ptr;

unsigned int hash(const char *key);
hash_map_ptr create_hash_map(void);
void insert(hash_map_ptr map, const char *key, int value);
int get(hash_map_ptr map, const char *key);
void remove_key(hash_map_ptr map, const char *key);
void free_hash_map(hash_map_ptr map);

#endif  // HASH_MAP_H_
