#include <types.h>
#include <dynaarray.h>
#include <linkedlist.h>
#ifndef MAP_H
#define MAP_H

typedef struct
{
    void *key;
    void *value;
    long key_size;
    long value_size;
} HashMapNode, *LPHashMapNode;

int _hashmapnode_new(void *key, void *value, long key_size, long value_size);
void _hashmapnode_free(LPHashMapNode lp_hmn, Boolean (*func)(void *));

typedef struct
{
    LPDynaArray items; // 存元素是LPHashMapNode的LinkedList实例
    int (*free_func)(void *);
    int (*copy_func)(void *, void *);
    int (*compare_func)(void *, void *);
} HashMap, *LPHashMap;

int HashMap_new(LPHashMap lp_hm);
int _hashmap_init(LPHashMap lp_map);
int HashMap_free(LPHashMap lp_map);

int HashMap_gen_hash_code(LPHashMap lp_map, void *key, long key_size);

int HashMap_put(LPHashMap lp_map, void *key, long key_size, void *value, long value_size);

int HashMap_get(LPHashMap lp_map, void *key, long key_size);

int HashMap_delete(LPHashMap lp_map, void *key, long key_size);

#endif