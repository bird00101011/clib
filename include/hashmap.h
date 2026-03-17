#ifndef HASHMAP_H
#define HASHMAP_H

#define TABLE_SIZE 32

typedef struct {
  long eles_num;
  LPDynaArray lp_da_lls;
  int (*copy_func)(void*, void*);
  int (*compare_func)(void*, void*);
  int (*free_func)(void*);
} HashMap, *LPHashMap;

typedef struct {
  void* key;
  void* value;
  long key_size;
  long value_size;
} HashMapNode, *LPHashMapNode;

__declspec(dllexport) int HashMap_init(LPHashMap lp_hm,
                                       int (*copy_func)(void*, void*),
                                       int (*compare_func)(void*, void*),
                                       int (*free_func)(void*));

__declspec(dllexport) unsigned long HashMap_gen_hashcode(void* key,
                                                         long key_size);

__declspec(dllexport) int HashMap_reallocate(LPHashMap lp_hm,
                                             long new_capacity);

__declspec(dllexport) int HashMap_put(LPHashMap lp_hm, LPHashMapNode lp_hmn);

__declspec(dllexport) int HashMap_get(LPHashMap lp_hm, LPHashMapNode lpmn);

__declspec(dllexport) int HashMap_del(LPHashMap lp_hm, void* key,
                                      long key_size);

__declspec(dllexport) int HashMap_free(LPHashMap lp_hm);

#endif  // HASHMAP_H