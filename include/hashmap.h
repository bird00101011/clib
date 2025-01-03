#include <types.h>
#include <arraylist.h>
#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct
{
    void *key;
    void *value;
    long value_size;
} HashMapNode;

typedef struct
{
    ArrayList *lp_hashmap_nodes;
} HashMap;

StatusDataError *hashmap_new(long capacity);

StatusDataError *hashmap_gen_hashcode(HashMap *lp_hashmap, void *key);

StatusDataError *hashmap_free(HashMap *lp_hashmap);

StatusDataError *hashmap_put(HashMap *lp_hashmap, HashMapNode *lp_hashmap_node);

StatusDataError *hashmap_del(HashMap *lp_hashmap, HashMapNode *lp_hashmap_node);

StatusDataError *hashmap_get(HashMap *lp_hashmap, void *key);

StatusDataError *hashmap_keys(HashMap *lp_hashmap);

StatusDataError *hashmap_values(HashMap *lp_hashmap);

StatusDataError *hashmap_iter(HashMap *lp_hashmap);

StatusDataError *hashmap_iter_stop(HashMap *lp_hashmap);

#endif