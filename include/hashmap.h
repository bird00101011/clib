#include <types.h>
#include <arraylist.h>
#include <linkedlist.h>
#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct
{
    Object key;
    Object value;
    long value_size;
} HashMapNode, *LPHashMapNode;

typedef struct
{
    LPArrayList lp_al_hashmap_linkedlists;
} HashMap, *LPHashMap;

LPStatusDataException HashMap_new(long capacity);

LPStatusDataException HashMap_gen_hashcode(LPHashMap lp_hashmap, Object key);

LPStatusDataException HashMap_free(LPHashMap lp_hashmap);

LPStatusDataException HashMap_put(LPHashMap lp_hashmap, LPHashMapNode lp_hashmap_node);

LPStatusDataException HashMap_del(LPHashMap lp_hashmap, LPHashMapNode lp_hashmap_node);

LPStatusDataException HashMap_get(LPHashMap lp_hashmap, Object key);

LPStatusDataException HashMap_keys(LPHashMap lp_hashmap);

LPStatusDataException HashMap_values(LPHashMap lp_hashmap);

LPStatusDataException HashMap_iter(LPHashMap lp_hashmap);

LPStatusDataException HashMap_iter_stop(LPHashMap lp_hashmap);

#endif