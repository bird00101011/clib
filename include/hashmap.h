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
} HashMapKV, *LPHashMapKV;

typedef struct
{
    LPDynaArray lp_lls;
    long kvs_num;
    int (*copy_func)(void *, void *);
    int (*compare_func)(void *, void *);
    int (*free_func)(void *);
} HashMap, *LPHashMap;

int HashMap_init(LPHashMap lp_hm,
                 int (*copy_func)(void *, void *),
                 int (*compare_func)(void *, void *),
                 int (*free_func)(void *));

int HashMap_free(LPHashMap lp_map);

int HashMap_gen_hash_code(LPHashMap lp_map, void *key, long key_size, long *lp_hsc);

int HashMap_put(LPHashMap lp_map, void *key, long key_size, void *value, long value_size);

int HashMap_get(LPHashMap lp_map, LPHashMapKV *ele);

int HashMap_delete(LPHashMap lp_map, void *key, long key_size);

#endif