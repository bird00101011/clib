#include <types.h>
#include <dynaarray.h>
#include <linkedlist.h>
#ifndef MAP_H
#define MAP_H

typedef struct
{
    Object key;
    Object value;
    long key_size;
    long value_size;
} HashMapNode, *LPHashMapNode;

LPStatusDataException _hashmapnode_new(Object key, Object value, long key_size, long value_size) void _hashmapnode_free(LPHashMapNode lp_hmn);
void _hashmapnode_free(LPHashMapNode lp_hmn)

    typedef struct
{
    LPDynaArray items; // 存元素是LPHashMapNode的LinkedList实例
} HashMap, *LPHashMap;

LPStatusDataException HashMap_new();
LPStatusDataException _hashmap_init(LPHashMap lp_map);
LPStatusDataException HashMap_free(LPHashMap lp_map);

LPStatusDataException HashMap_gen_hash_code(LPHashMap lp_map, Object key, long key_size);

LPStatusDataException HashMap_put(LPHashMap lp_map, Object key, long key_size, Object value, long value_size);

LPStatusDataException HashMap_get(LPHashMap lp_map, Object key, long key_size);

LPStatusDataException HashMap_delete(LPHashMap lp_map, Object key, long key_size);

#endif