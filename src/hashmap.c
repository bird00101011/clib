#include <assert.h>
#include <dynaarray.h>
#include <error.h>
#include <hashmap.h>
#include <linkedlist.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

__declspec(dllexport) int HashMap_init(LPHashMap lp_hm,
                                       int (*copy_func)(void *, void *),
                                       int (*compare_func)(void *, void *),
                                       int (*free_func)(void *)) {
  assert(lp_hm != NULL_POINTER);

  return FALSE;
}