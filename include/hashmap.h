#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct {
  long eles_num;
  LPDynaArray lp_da_lls;
  int (*copy_func)(void *, void *);
  int (*compare_func)(void *, void *);
  int (*free_func)(void *);
} HashMap, *LPHashMap;

__declspec(dllexport) int HashMap_init(LPHashMap lp_hm,
                                       int (*copy_func)(void *, void *),
                                       int (*compare_func)(void *, void *),
                                       int (*free_func)(void *));
#endif  // HASHMAP_H