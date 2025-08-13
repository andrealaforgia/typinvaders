#include "hash_map.h"

#include <stdlib.h>
#include <string.h>

#include "strdup.h"

unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key;
        key++;
    }
    return hash % table_size;
}

hash_map_t create_hash_map(void) {
    hash_map_t map;
    map.table = malloc(table_size * sizeof(node_t));
    for (int i = 0; i < table_size; i++) {
        map.table[i] = NULL;
    }
    return map;
}

void insert(hash_map_ptr map, const char *key, void *value) {
    unsigned int index = hash(key);
    node_ptr new_node = malloc(sizeof(node_t));
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = map->table[index];
    map->table[index] = new_node;
}

void *get(hash_map_ptr map, const char *key) {
    unsigned int index = hash(key);
    node_ptr current = map->table[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;  // Key not found
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
