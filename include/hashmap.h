#include <dynaarray.h>
#include <linkedlist.h>
#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct
{
    void *key;
    void *value;
    long key_size;
    long value_size;
} HashMapNode, *LPHashMapNode;

typedef struct
{
    LPDynaArray lp_lls;
    int (*free_func)(void *);
    int (*copy_func)(void *, void *);
    int (*compare_func)(void *, void *);
} HashMap, *LPHashMap;

int HashMap_new(LPHashMap lp_hm);
int HashMap_free(LPHashMap lp_map);

int HashMap_gen_hash_code(LPHashMap lp_map, void *key, long key_size);

int HashMap_put(LPHashMap lp_map, void *key, long key_size, void *value, long value_size);

int HashMap_get(LPHashMap lp_map, void *key, long key_size);

int HashMap_delete(LPHashMap lp_map, void *key, long key_size);

#endif